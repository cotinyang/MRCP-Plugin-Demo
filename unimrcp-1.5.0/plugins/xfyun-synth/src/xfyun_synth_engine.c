/*
 * Copyright 2008-2015 Arsen Chaloyan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * Mandatory rules concerning plugin implementation.
 * 1. Each plugin MUST implement a plugin/engine creator function
 *    with the exact signature and name (the main entry point)
 *        MRCP_PLUGIN_DECLARE(mrcp_engine_t*) mrcp_plugin_create(apr_pool_t *pool)
 * 2. Each plugin MUST declare its version number
 *        MRCP_PLUGIN_VERSION_DECLARE
 * 3. One and only one response MUST be sent back to the received request.
 * 4. Methods (callbacks) of the MRCP engine channel MUST not block.
 *   (asynchronous response can be sent from the context of other thread)
 * 5. Methods (callbacks) of the MPF engine stream MUST not block.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

#include "mrcp_synth_engine.h"
#include "apt_consumer_task.h"
#include "apt_log.h"
#include "mpf_buffer.h"

#define SYNTH_ENGINE_TASK_NAME "XFyun Synth Engine"

typedef struct xfyun_synth_engine_t xfyun_synth_engine_t;
typedef struct xfyun_synth_channel_t xfyun_synth_channel_t;
typedef struct xfyun_synth_msg_t xfyun_synth_msg_t;

/** Declaration of synthesizer engine methods */
static apt_bool_t xfyun_synth_engine_destroy(mrcp_engine_t *engine);
static apt_bool_t xfyun_synth_engine_open(mrcp_engine_t *engine);
static apt_bool_t xfyun_synth_engine_close(mrcp_engine_t *engine);
static mrcp_engine_channel_t* xfyun_synth_engine_channel_create(mrcp_engine_t *engine, apr_pool_t *pool);

static const struct mrcp_engine_method_vtable_t engine_vtable = {
	xfyun_synth_engine_destroy,
	xfyun_synth_engine_open,
	xfyun_synth_engine_close,
	xfyun_synth_engine_channel_create
};


/** Declaration of synthesizer channel methods */
static apt_bool_t xfyun_synth_channel_destroy(mrcp_engine_channel_t *channel);
static apt_bool_t xfyun_synth_channel_open(mrcp_engine_channel_t *channel);
static apt_bool_t xfyun_synth_channel_close(mrcp_engine_channel_t *channel);
static apt_bool_t xfyun_synth_channel_request_process(mrcp_engine_channel_t *channel, mrcp_message_t *request);

static const struct mrcp_engine_channel_method_vtable_t channel_vtable = {
	xfyun_synth_channel_destroy,
	xfyun_synth_channel_open,
	xfyun_synth_channel_close,
	xfyun_synth_channel_request_process
};

/** Declaration of synthesizer audio stream methods */
static apt_bool_t xfyun_synth_stream_destroy(mpf_audio_stream_t *stream);
static apt_bool_t xfyun_synth_stream_open(mpf_audio_stream_t *stream, mpf_codec_t *codec);
static apt_bool_t xfyun_synth_stream_close(mpf_audio_stream_t *stream);
static apt_bool_t xfyun_synth_stream_read(mpf_audio_stream_t *stream, mpf_frame_t *frame);

static const mpf_audio_stream_vtable_t audio_stream_vtable = {
	xfyun_synth_stream_destroy,
	xfyun_synth_stream_open,
	xfyun_synth_stream_close,
	xfyun_synth_stream_read,
	NULL,
	NULL,
	NULL,
	NULL
};

/** Declaration of xfyun synthesizer engine */
struct xfyun_synth_engine_t {
	apt_consumer_task_t    *task;
};

/** Declaration of xfyun synthesizer channel */
struct xfyun_synth_channel_t {
	/** Back pointer to engine */
	xfyun_synth_engine_t   *xfyun_engine;
	/** Engine channel base */
	mrcp_engine_channel_t *channel;

	/** Active (in-progress) speak request */
	mrcp_message_t        *speak_request;
	/** Pending stop response */
	mrcp_message_t        *stop_response;
	/** Estimated time to complete */
	apr_size_t             time_to_complete;
	/** Is paused */
	apt_bool_t             paused;
	/** Speech source (used instead of actual synthesis) */

	mpf_buffer_t          *audio_buffer;

};

typedef enum {
	XFYUN_SYNTH_MSG_OPEN_CHANNEL,
	XFYUN_SYNTH_MSG_CLOSE_CHANNEL,
	XFYUN_SYNTH_MSG_REQUEST_PROCESS
} xfyun_synth_msg_type_e;

/** Declaration of xfyun synthesizer task message */
struct xfyun_synth_msg_t {
	xfyun_synth_msg_type_e  type;
	mrcp_engine_channel_t *channel; 
	mrcp_message_t        *request;
};


static apt_bool_t xfyun_synth_msg_signal(xfyun_synth_msg_type_e type, mrcp_engine_channel_t *channel, mrcp_message_t *request);
static apt_bool_t xfyun_synth_msg_process(apt_task_t *task, apt_task_msg_t *msg);
static void xfyun_synth_on_start(apt_task_t *task);
static void xfyun_synth_on_terminate(apt_task_t *task);

/** Declare this macro to set plugin version */
MRCP_PLUGIN_VERSION_DECLARE

/**
 * Declare this macro to use log routine of the server, plugin is loaded from.
 * Enable/add the corresponding entry in logger.xml to set a cutsom log source priority.
 *    <source name="SYNTH-PLUGIN" priority="DEBUG" masking="NONE"/>
 */
MRCP_PLUGIN_LOG_SOURCE_IMPLEMENT(SYNTH_PLUGIN,"SYNTH-PLUGIN")

/** Use custom log source mark */
#define SYNTH_LOG_MARK   APT_LOG_MARK_DECLARE(SYNTH_PLUGIN)

/** Create xfyun synthesizer engine */
MRCP_PLUGIN_DECLARE(mrcp_engine_t*) mrcp_plugin_create(apr_pool_t *pool)
{
	/* create xfyun engine */
	xfyun_synth_engine_t *xfyun_engine = apr_palloc(pool,sizeof(xfyun_synth_engine_t));
	apt_task_t *task;
	apt_task_vtable_t *vtable;
	apt_task_msg_pool_t *msg_pool;

	/* create task/thread to run xfyun engine in the context of this task */
	msg_pool = apt_task_msg_pool_create_dynamic(sizeof(xfyun_synth_msg_t),pool);
	xfyun_engine->task = apt_consumer_task_create(xfyun_engine,msg_pool,pool);
	if(!xfyun_engine->task) {
		return NULL;
	}
	task = apt_consumer_task_base_get(xfyun_engine->task);
	apt_task_name_set(task,SYNTH_ENGINE_TASK_NAME);
	vtable = apt_task_vtable_get(task);
	if(vtable) {
		vtable->process_msg = xfyun_synth_msg_process;
		// vtable->on_pre_run = xfyun_synth_on_start;
		// vtable->on_post_run = xfyun_synth_on_terminate;
	}

	/* create engine base */
	return mrcp_engine_create(
				MRCP_SYNTHESIZER_RESOURCE, /* MRCP resource identifier */
				xfyun_engine,               /* object to associate */
				&engine_vtable,            /* virtual methods table of engine */
				pool);                     /* pool to allocate memory from */
}

/** Destroy synthesizer engine */
static apt_bool_t xfyun_synth_engine_destroy(mrcp_engine_t *engine)
{
	xfyun_synth_engine_t *xfyun_engine = engine->obj;
	if(xfyun_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(xfyun_engine->task);
		apt_task_destroy(task);
		xfyun_engine->task = NULL;
	}
	return TRUE;
}

/** Open synthesizer engine */
static apt_bool_t xfyun_synth_engine_open(mrcp_engine_t *engine)
{
	xfyun_synth_engine_t *xfyun_engine = engine->obj;
	if(xfyun_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(xfyun_engine->task);
		apt_task_start(task);
	}
	return mrcp_engine_open_respond(engine,TRUE);
}

/** Close synthesizer engine */
static apt_bool_t xfyun_synth_engine_close(mrcp_engine_t *engine)
{
	xfyun_synth_engine_t *xfyun_engine = engine->obj;
	if(xfyun_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(xfyun_engine->task);
		apt_task_terminate(task,TRUE);
	}
	return mrcp_engine_close_respond(engine);
}

/** Create xfyun synthesizer channel derived from engine channel base */
static mrcp_engine_channel_t* xfyun_synth_engine_channel_create(mrcp_engine_t *engine, apr_pool_t *pool)
{
	apt_log(APT_LOG_MARK, APT_PRIO_INFO, "[xfyun tts] create synthesizer channel");
	mpf_stream_capabilities_t *capabilities;
	mpf_termination_t *termination; 

	/* create xfyun synth channel */
	xfyun_synth_channel_t *synth_channel = apr_palloc(pool,sizeof(xfyun_synth_channel_t));
	synth_channel->xfyun_engine = engine->obj;
	synth_channel->speak_request = NULL;
	synth_channel->stop_response = NULL;
	synth_channel->time_to_complete = 0;
	synth_channel->paused = FALSE;
	synth_channel->audio_buffer = NULL;
	
	capabilities = mpf_source_stream_capabilities_create(pool);
	mpf_codec_capabilities_add(
			&capabilities->codecs,
			MPF_SAMPLE_RATE_8000 | MPF_SAMPLE_RATE_16000,
			"LPCM");

	/* create media termination */
	termination = mrcp_engine_audio_termination_create(
			synth_channel,        /* object to associate */
			&audio_stream_vtable, /* virtual methods table of audio stream */
			capabilities,         /* stream capabilities */
			pool);                /* pool to allocate memory from */

	/* create engine channel base */
	synth_channel->channel = mrcp_engine_channel_create(
			engine,               /* engine */
			&channel_vtable,      /* virtual methods table of engine channel */
			synth_channel,        /* object to associate */
			termination,          /* associated media termination */
			pool);                /* pool to allocate memory from */

	synth_channel->audio_buffer = mpf_buffer_create(pool);
	return synth_channel->channel;
}

/** Destroy engine channel */
static apt_bool_t xfyun_synth_channel_destroy(mrcp_engine_channel_t *channel)
{
	/* nothing to destroy */
	return TRUE;
}

/** Open engine channel (asynchronous response MUST be sent)*/
static apt_bool_t xfyun_synth_channel_open(mrcp_engine_channel_t *channel)
{
	return xfyun_synth_msg_signal(XFYUN_SYNTH_MSG_OPEN_CHANNEL,channel,NULL);
}

/** Close engine channel (asynchronous response MUST be sent)*/
static apt_bool_t xfyun_synth_channel_close(mrcp_engine_channel_t *channel)
{
	return xfyun_synth_msg_signal(XFYUN_SYNTH_MSG_CLOSE_CHANNEL,channel,NULL);
}

/** Process MRCP channel request (asynchronous response MUST be sent)*/
static apt_bool_t xfyun_synth_channel_request_process(mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
	return xfyun_synth_msg_signal(XFYUN_SYNTH_MSG_REQUEST_PROCESS,channel,request);
}

static apt_bool_t xfyun_synth_text_to_speech(const char* src_text, const char* params, mpf_buffer_t *buffer) {
	int ret = -1;
	const char*  sessionID = NULL;
	int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
	unsigned int audio_len = 0;

	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		apt_log(APT_LOG_MARK, APT_PRIO_WARNING,"[xfyun] QTTSSessionBegin failed, error code: %d.", ret);
		return FALSE;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		apt_log(APT_LOG_MARK, APT_PRIO_WARNING,"[xfyun] QTTSTextPut failed, error code: %d.",ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		return FALSE;
	}
	apt_log(APT_LOG_MARK, APT_PRIO_WARNING,"[xfyun] 正在合成 ...");
	while (1) 
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			mpf_buffer_audio_write(buffer, data, audio_len);
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
		usleep(150*1000); //防止频繁占用CPU
	}
	if (MSP_SUCCESS != ret)
	{
		apt_log(APT_LOG_MARK, APT_PRIO_WARNING,"[xfyun] QTTSAudioGet failed, error code: %d.",ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		return FALSE;
	}
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		apt_log(APT_LOG_MARK, APT_PRIO_WARNING,"[xfyun] QTTSSessionEnd failed, error code: %d.",ret);
		return FALSE;
	}
	return TRUE;
}

/** Process SPEAK request */
static apt_bool_t synth_response_construct(mrcp_message_t *response, mrcp_status_code_e status_code, mrcp_synth_completion_cause_e completion_cause)
{
	mrcp_synth_header_t *synth_header = mrcp_resource_header_prepare(response);
	if(!synth_header) {
		return FALSE;
	}

	response->start_line.status_code = status_code;
	synth_header->completion_cause = completion_cause;
	mrcp_resource_header_property_add(response,SYNTHESIZER_HEADER_COMPLETION_CAUSE);
	return TRUE;
}

/** Process SPEAK request */
static apt_bool_t xfyun_synth_channel_speak(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	apt_log(APT_LOG_MARK, APT_PRIO_INFO, "[xfyun tts] process speak request");
	apt_str_t *body;
	const char* session_begin_params = "voice_name = xiaoyan, text_encoding = utf8, sample_rate = 8000, speed = 50, volume = 50, pitch = 50, rdn = 2";
	xfyun_synth_channel_t *synth_channel = channel->method_obj;
	const mpf_codec_descriptor_t *descriptor = mrcp_engine_source_stream_codec_get(channel);

	if(!descriptor) {
		apt_log(SYNTH_LOG_MARK,APT_PRIO_WARNING,"Failed to Get Codec Descriptor " APT_SIDRES_FMT, MRCP_MESSAGE_SIDRES(request));
		response->start_line.status_code = MRCP_STATUS_CODE_METHOD_FAILED;
		return FALSE;
	}

	synth_channel->speak_request = request;
	body = &synth_channel->speak_request->body;
	if(!body->length) {
		synth_channel->speak_request = NULL;
		synth_response_construct(response,MRCP_STATUS_CODE_MISSING_PARAM,SYNTHESIZER_COMPLETION_CAUSE_ERROR);
		mrcp_engine_channel_message_send(synth_channel->channel,response);
		return FALSE;
	}

	synth_channel->time_to_complete = 0;

	response->start_line.request_state = MRCP_REQUEST_STATE_INPROGRESS;
	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);

	xfyun_synth_text_to_speech(body->buf, session_begin_params, synth_channel->audio_buffer); 
	mpf_buffer_event_write(synth_channel->audio_buffer, MEDIA_FRAME_TYPE_EVENT);
	return TRUE;
}

static APR_INLINE xfyun_synth_channel_t* xfyun_synth_channel_get(apt_task_t *task)
{
	apt_consumer_task_t *consumer_task = apt_task_object_get(task);
	return apt_consumer_task_object_get(consumer_task);
}

static void xfyun_synth_on_start(apt_task_t *task)
{
	xfyun_synth_channel_t *synth_channel = xfyun_synth_channel_get(task);
	apt_log(APT_LOG_MARK, APT_PRIO_DEBUG, "Speak task started"); 
	mrcp_engine_channel_open_respond(synth_channel->channel,TRUE);
}

static void xfyun_synth_on_terminate(apt_task_t *task)
{
	xfyun_synth_channel_t *synth_channel = xfyun_synth_channel_get(task);
	apt_log(APT_LOG_MARK, APT_PRIO_DEBUG, "Speak task terminated");
	mrcp_engine_channel_close_respond(synth_channel->channel);
}

/** Process STOP request */
static apt_bool_t xfyun_synth_channel_stop(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	xfyun_synth_channel_t *synth_channel = channel->method_obj;
	/* store the request, make sure there is no more activity and only then send the response */
	synth_channel->stop_response = response;
	return TRUE;
}

/** Process PAUSE request */
static apt_bool_t xfyun_synth_channel_pause(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	xfyun_synth_channel_t *synth_channel = channel->method_obj;
	synth_channel->paused = TRUE;
	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);
	return TRUE;
}

/** Process RESUME request */
static apt_bool_t xfyun_synth_channel_resume(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	xfyun_synth_channel_t *synth_channel = channel->method_obj;
	synth_channel->paused = FALSE;
	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);
	return TRUE;
}

/** Process SET-PARAMS request */
static apt_bool_t xfyun_synth_channel_set_params(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	mrcp_synth_header_t *req_synth_header;
	/* get synthesizer header */
	req_synth_header = mrcp_resource_header_get(request);
	if(req_synth_header) {
		/* check voice age header */
		if(mrcp_resource_header_property_check(request,SYNTHESIZER_HEADER_VOICE_AGE) == TRUE) {
			apt_log(SYNTH_LOG_MARK,APT_PRIO_INFO,"Set Voice Age [%"APR_SIZE_T_FMT"]",
				req_synth_header->voice_param.age);
		}
		/* check voice name header */
		if(mrcp_resource_header_property_check(request,SYNTHESIZER_HEADER_VOICE_NAME) == TRUE) {
			apt_log(SYNTH_LOG_MARK,APT_PRIO_INFO,"Set Voice Name [%s]",
				req_synth_header->voice_param.name.buf);
		}
	}
	
	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);
	return TRUE;
}

/** Process GET-PARAMS request */
static apt_bool_t xfyun_synth_channel_get_params(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	mrcp_synth_header_t *req_synth_header;
	/* get synthesizer header */
	req_synth_header = mrcp_resource_header_get(request);
	if(req_synth_header) {
		mrcp_synth_header_t *res_synth_header = mrcp_resource_header_prepare(response);
		/* check voice age header */
		if(mrcp_resource_header_property_check(request,SYNTHESIZER_HEADER_VOICE_AGE) == TRUE) {
			res_synth_header->voice_param.age = 25;
			mrcp_resource_header_property_add(response,SYNTHESIZER_HEADER_VOICE_AGE);
		}
		/* check voice name header */
		if(mrcp_resource_header_property_check(request,SYNTHESIZER_HEADER_VOICE_NAME) == TRUE) {
			apt_string_set(&res_synth_header->voice_param.name,"David");
			mrcp_resource_header_property_add(response,SYNTHESIZER_HEADER_VOICE_NAME);
		}
	}

	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);
	return TRUE;
}

/** Dispatch MRCP request */
static apt_bool_t xfyun_synth_channel_request_dispatch(mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
	apt_bool_t processed = FALSE;
	mrcp_message_t *response = mrcp_response_create(request,request->pool);
	switch(request->start_line.method_id) {
		case SYNTHESIZER_SET_PARAMS:
			processed = xfyun_synth_channel_set_params(channel,request,response);
			break;
		case SYNTHESIZER_GET_PARAMS:
			processed = xfyun_synth_channel_get_params(channel,request,response);
			break;
		case SYNTHESIZER_SPEAK:
			processed = xfyun_synth_channel_speak(channel,request,response);
			break;
		case SYNTHESIZER_STOP:
			processed = xfyun_synth_channel_stop(channel,request,response);
			break;
		case SYNTHESIZER_PAUSE:
			processed = xfyun_synth_channel_pause(channel,request,response);
			break;
		case SYNTHESIZER_RESUME:
			processed = xfyun_synth_channel_resume(channel,request,response);
			break;
		case SYNTHESIZER_BARGE_IN_OCCURRED:
			processed = xfyun_synth_channel_stop(channel,request,response);
			break;
		case SYNTHESIZER_CONTROL:
			break;
		case SYNTHESIZER_DEFINE_LEXICON:
			break;
		default:
			break;
	}
	if(processed == FALSE) {
		/* send asynchronous response for not handled request */
		mrcp_engine_channel_message_send(channel,response);
	}
	return TRUE;
}

/** Callback is called from MPF engine context to destroy any additional data associated with audio stream */
static apt_bool_t xfyun_synth_stream_destroy(mpf_audio_stream_t *stream)
{
	return TRUE;
}

/** Callback is called from MPF engine context to perform any action before open */
static apt_bool_t xfyun_synth_stream_open(mpf_audio_stream_t *stream, mpf_codec_t *codec)
{
	return TRUE;
}

/** Callback is called from MPF engine context to perform any action after close */
static apt_bool_t xfyun_synth_stream_close(mpf_audio_stream_t *stream)
{
	return TRUE;
}

/** Raise SPEAK-COMPLETE event */
static apt_bool_t xfyun_synth_speak_complete_raise(xfyun_synth_channel_t *synth_channel)
{
	mrcp_message_t *message = 0;
	mrcp_synth_header_t * synth_header = 0;
	apt_log(APT_LOG_MARK, APT_PRIO_DEBUG, "[xfyun tts] xfyun_synth_speak_complete_raise");

	if (!synth_channel->speak_request) {
		return FALSE;
	}

	message = mrcp_event_create(
						synth_channel->speak_request,
						SYNTHESIZER_SPEAK_COMPLETE,
						synth_channel->speak_request->pool);
	if (!message) {
		return FALSE;
	}

	/* get/allocate synthesizer header */
	synth_header = (mrcp_synth_header_t *) mrcp_resource_header_prepare(message);
	if (synth_header) {
		/* set completion cause */
		synth_header->completion_cause = SYNTHESIZER_COMPLETION_CAUSE_NORMAL;
		mrcp_resource_header_property_add(message,SYNTHESIZER_HEADER_COMPLETION_CAUSE);
	}
	/* set request state */
	message->start_line.request_state = MRCP_REQUEST_STATE_COMPLETE;

	synth_channel->speak_request = NULL;
	/* send asynch event */
	return mrcp_engine_channel_message_send(synth_channel->channel,message);
}

/** Callback is called from MPF engine context to read/get new frame */
static apt_bool_t xfyun_synth_stream_read(mpf_audio_stream_t *stream, mpf_frame_t *frame)
{
	// apt_log(APT_LOG_MARK, APT_PRIO_INFO, "[xfyun tts] stream read");
	xfyun_synth_channel_t *synth_channel = stream->obj;
	/* check if STOP was requested */
	if(synth_channel->stop_response) {
		/* send asynchronous response to STOP request */
		mrcp_engine_channel_message_send(synth_channel->channel,synth_channel->stop_response);
		synth_channel->stop_response = NULL;
		synth_channel->speak_request = NULL;
		synth_channel->paused = FALSE;
		return TRUE;
	}

	/* check if there is active SPEAK request and it isn't in paused state */
	if(synth_channel->speak_request && synth_channel->paused == FALSE) {
		// apt_log(APT_LOG_MARK, APT_PRIO_INFO, "[xfyun tts] read audio buffer to frame");
		/* normal processing */
		mpf_buffer_frame_read(synth_channel->audio_buffer,frame);
			/* raise SPEAK-COMPLETE event */
		if((frame->type & MEDIA_FRAME_TYPE_EVENT) == MEDIA_FRAME_TYPE_EVENT) {
			frame->type &= ~MEDIA_FRAME_TYPE_EVENT;
			xfyun_synth_speak_complete_raise(synth_channel);
		}
	}
	return TRUE;
}

static apt_bool_t xfyun_synth_msg_signal(xfyun_synth_msg_type_e type, mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
	apt_bool_t status = FALSE;
	xfyun_synth_channel_t *xfyun_channel = channel->method_obj;
	xfyun_synth_engine_t *xfyun_engine = xfyun_channel->xfyun_engine;
	apt_task_t *task = apt_consumer_task_base_get(xfyun_engine->task);
	apt_task_msg_t *msg = apt_task_msg_get(task);
	if(msg) {
		xfyun_synth_msg_t *xfyun_msg;
		msg->type = TASK_MSG_USER;
		xfyun_msg = (xfyun_synth_msg_t*) msg->data;

		xfyun_msg->type = type;
		xfyun_msg->channel = channel;
		xfyun_msg->request = request;
		status = apt_task_msg_signal(task,msg);
	}
	return status;
}

static apt_bool_t xfyun_synth_msg_process(apt_task_t *task, apt_task_msg_t *msg)
{
	xfyun_synth_msg_t *xfyun_msg = (xfyun_synth_msg_t*)msg->data;
	switch(xfyun_msg->type) {
		case XFYUN_SYNTH_MSG_OPEN_CHANNEL:
			/* open channel and send asynch response */
			mrcp_engine_channel_open_respond(xfyun_msg->channel,TRUE);
			break;
		case XFYUN_SYNTH_MSG_CLOSE_CHANNEL:
			/* close channel, make sure there is no activity and send asynch response */
			mrcp_engine_channel_close_respond(xfyun_msg->channel);
			break;
		case XFYUN_SYNTH_MSG_REQUEST_PROCESS:
			xfyun_synth_channel_request_dispatch(xfyun_msg->channel,xfyun_msg->request);
			break;
		default:
			break;
	}
	return TRUE;
}
