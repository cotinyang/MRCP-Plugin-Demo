
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
#include <stdlib.h>
#include <pthread.h>
#include "aneex_recog_engine.h"
#include "mpf_activity_detector.h"
#include "apt_consumer_task.h"
#include "apt_log.h"

#include "AneexDefine.h"

#define ANEEX_ENGINE_TASK_NAME "Aneex Recog Engine"

typedef struct aneex_recog_engine_t aneex_recog_engine_t;
typedef struct aneex_recog_channel_t aneex_recog_channel_t;
typedef struct aneex_recog_msg_t aneex_recog_msg_t;

/** Declaration of recognizer engine methods */
static apt_bool_t aneex_recog_engine_destroy(mrcp_engine_t *engine);
static apt_bool_t aneex_recog_engine_open(mrcp_engine_t *engine);
static apt_bool_t aneex_recog_engine_close(mrcp_engine_t *engine);
static mrcp_engine_channel_t* aneex_recog_engine_channel_create(mrcp_engine_t *engine, apr_pool_t *pool);

static const struct mrcp_engine_method_vtable_t engine_vtable = {
	aneex_recog_engine_destroy,
	aneex_recog_engine_open,
	aneex_recog_engine_close,
	aneex_recog_engine_channel_create
};


/** Declaration of recognizer channel methods */
static apt_bool_t aneex_recog_channel_destroy(mrcp_engine_channel_t *channel);
static apt_bool_t aneex_recog_channel_open(mrcp_engine_channel_t *channel);
static apt_bool_t aneex_recog_channel_close(mrcp_engine_channel_t *channel);
static apt_bool_t aneex_recog_channel_request_process(mrcp_engine_channel_t *channel, mrcp_message_t *request);

static const struct mrcp_engine_channel_method_vtable_t channel_vtable = {
	aneex_recog_channel_destroy,
	aneex_recog_channel_open,
	aneex_recog_channel_close,
	aneex_recog_channel_request_process
};

/** Declaration of recognizer audio stream methods */
static apt_bool_t aneex_recog_stream_destroy(mpf_audio_stream_t *stream);
static apt_bool_t aneex_recog_stream_open(mpf_audio_stream_t *stream, mpf_codec_t *codec);
static apt_bool_t aneex_recog_stream_close(mpf_audio_stream_t *stream);
static apt_bool_t aneex_recog_stream_write(mpf_audio_stream_t *stream, const mpf_frame_t *frame);

static const mpf_audio_stream_vtable_t audio_stream_vtable = {
	aneex_recog_stream_destroy,
	NULL,
	NULL,
	NULL,
	aneex_recog_stream_open,
	aneex_recog_stream_close,
	aneex_recog_stream_write,
	NULL
};

/** Declaration of aneex recognizer engine */
struct aneex_recog_engine_t {
	apt_consumer_task_t    *task;
};

/** Declaration of aneex recognizer channel */
struct aneex_recog_channel_t {
	/** Back pointer to engine */
	aneex_recog_engine_t     *aneex_engine;
	/** Engine channel base */
	mrcp_engine_channel_t   *channel;

	/** Active (in-progress) recognition request */
	mrcp_message_t          *recog_request;
	/** Pending stop response */
	mrcp_message_t          *stop_response;
	/** Indicates whether input timers are started */
	apt_bool_t               timers_started;
	/** Voice activity detector */
	mpf_activity_detector_t *detector;
	/** File to write utterance to */
	FILE                    *audio_out;
};

typedef enum {
	ANEEX_RECOG_MSG_OPEN_CHANNEL,
	ANEEX_RECOG_MSG_CLOSE_CHANNEL,
	ANEEX_RECOG_MSG_REQUEST_PROCESS
} aneex_recog_msg_type_e;

/** Declaration of aneex recognizer task message */
struct aneex_recog_msg_t {
	aneex_recog_msg_type_e  type;
	mrcp_engine_channel_t *channel; 
	mrcp_message_t        *request;
};

static apt_bool_t aneex_recog_msg_signal(aneex_recog_msg_type_e type, mrcp_engine_channel_t *channel, mrcp_message_t *request);
static apt_bool_t aneex_recog_msg_process(apt_task_t *task, apt_task_msg_t *msg);

// ---------структуры данных для потока---------
char *audio_file_name;
char *audio_file_path;
char *db_file_path="/usr/local/unimrcp/data/DB";

typedef struct{
	char *audio_path;
	char *db_path;
} pthrData;

pthread_mutex_t lock; 	//Исключающая блокировка
pthread_t thread;		//поток
pthrData threadData; 	//структура

void* threadFunc(void* thread_data);

void aneex_recog_from_db(char *audio_path, char* db_path, aneex_recog_channel_t *recog_channel);

//для проверки итогов распознавания
int result=0;
// ---------структуры данных для потока---------

/** Declare this macro to set plugin version */
MRCP_PLUGIN_VERSION_DECLARE

/**
 * Declare this macro to use log routine of the server, plugin is loaded from.
 * Enable/add the corresponding entry in logger.xml to set a cutsom log source priority.
 *    <source name="RECOG-PLUGIN" priority="DEBUG" masking="NONE"/>
 */
MRCP_PLUGIN_LOG_SOURCE_IMPLEMENT(ANEEX_PLUGIN,"ANNEX-PLUGIN")

/** Use custom log source mark */
#define ANEEX_LOG_MARK   APT_LOG_MARK_DECLARE(ANEEX_PLUGIN)

/** Create aneex recognizer engine */
MRCP_PLUGIN_DECLARE(mrcp_engine_t*) mrcp_plugin_create(apr_pool_t *pool)
{


	aneex_recog_engine_t *aneex_engine = apr_palloc(pool,sizeof(aneex_recog_engine_t));
	apt_task_t *task;
	apt_task_vtable_t *vtable;
	apt_task_msg_pool_t *msg_pool;

	msg_pool = apt_task_msg_pool_create_dynamic(sizeof(aneex_recog_msg_t),pool);
	aneex_engine->task = apt_consumer_task_create(aneex_engine,msg_pool,pool);
	if(!aneex_engine->task) {
		return NULL;
	}
	task = apt_consumer_task_base_get(aneex_engine->task);
	apt_task_name_set(task,ANEEX_ENGINE_TASK_NAME);
	vtable = apt_task_vtable_get(task);
	if(vtable) {
		vtable->process_msg = aneex_recog_msg_process;
	}

	/* create engine base */
	return mrcp_engine_create(
				//ANEEX_RECOGNIZER_RESOURCE,  /* MRCP resource identifier */
				MRCP_RECOGNIZER_RESOURCE,  /* MRCP resource identifier */
				aneex_engine,               /* object to associate */
				&engine_vtable,            /* virtual methods table of engine */
				pool);                     /* pool to allocate memory from */
}

/** Destroy recognizer engine */
static apt_bool_t aneex_recog_engine_destroy(mrcp_engine_t *engine)
{
	aneex_recog_engine_t *aneex_engine = engine->obj;
	if(aneex_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(aneex_engine->task);
		apt_task_destroy(task);
		aneex_engine->task = NULL;
	}
	return TRUE;
}

/** Open recognizer engine */
static apt_bool_t aneex_recog_engine_open(mrcp_engine_t *engine)
{
	aneex_recog_engine_t *aneex_engine = engine->obj;
	if(aneex_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(aneex_engine->task);
		apt_task_start(task);
	}
	return mrcp_engine_open_respond(engine,TRUE);
}

/** Close recognizer engine */
static apt_bool_t aneex_recog_engine_close(mrcp_engine_t *engine)
{
	aneex_recog_engine_t *aneex_engine = engine->obj;
	if(aneex_engine->task) {
		apt_task_t *task = apt_consumer_task_base_get(aneex_engine->task);
		apt_task_terminate(task,TRUE);
	}

	return mrcp_engine_close_respond(engine);
}

static mrcp_engine_channel_t* aneex_recog_engine_channel_create(mrcp_engine_t *engine, apr_pool_t *pool)
{
    printf("DEBUG: Plugin: aneex_recog_engine_channel_create\n");

	mpf_stream_capabilities_t *capabilities;
	mpf_termination_t *termination; 

	/* create aneex recog channel */
	aneex_recog_channel_t *recog_channel = apr_palloc(pool,sizeof(aneex_recog_channel_t));
	recog_channel->aneex_engine = engine->obj;
	recog_channel->recog_request = NULL;
	recog_channel->stop_response = NULL;
	recog_channel->detector = mpf_activity_detector_create(pool);
	recog_channel->audio_out = NULL;

	capabilities = mpf_sink_stream_capabilities_create(pool);
	mpf_codec_capabilities_add(
			&capabilities->codecs,
			MPF_SAMPLE_RATE_8000 | MPF_SAMPLE_RATE_16000,
			"L16");

	//create link to DB file in /data

	/* create media termination */
	termination = mrcp_engine_audio_termination_create(
			recog_channel,        /* object to associate */
			&audio_stream_vtable, /* virtual methods table of audio stream */
			capabilities,         /* stream capabilities */
			pool);                /* pool to allocate memory from */

	/* create engine channel base */
	recog_channel->channel = mrcp_engine_channel_create(
			engine,               /* engine */
			&channel_vtable,      /* virtual methods table of engine channel */
			recog_channel,        /* object to associate */
			termination,          /* associated media termination */
			pool);                /* pool to allocate memory from */

	return recog_channel->channel;
}

/** Destroy engine channel */
static apt_bool_t aneex_recog_channel_destroy(mrcp_engine_channel_t *channel)
{
	/* nothing to destrtoy */
	return TRUE;
}

/** Open engine channel (asynchronous response MUST be sent)*/
static apt_bool_t aneex_recog_channel_open(mrcp_engine_channel_t *channel)
{
	result=0;
	audio_file_name="";
	audio_file_path="";
	return aneex_recog_msg_signal(ANEEX_RECOG_MSG_OPEN_CHANNEL,channel,NULL);
}

/** Close engine channel (asynchronous response MUST be sent)*/
static apt_bool_t aneex_recog_channel_close(mrcp_engine_channel_t *channel)
{
	return aneex_recog_msg_signal(ANEEX_RECOG_MSG_CLOSE_CHANNEL,channel,NULL);
}

/** Process MRCP channel request (asynchronous response MUST be sent)*/
static apt_bool_t aneex_recog_channel_request_process(mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
	return aneex_recog_msg_signal(ANEEX_RECOG_MSG_REQUEST_PROCESS,channel,request);
}

/** Process RECOGNIZE request */
static apt_bool_t aneex_recog_channel_recognize(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	/* process RECOGNIZE request */
	aneex_recog_header_t *recog_header;
	aneex_recog_channel_t *recog_channel = channel->method_obj;
	const mpf_codec_descriptor_t *descriptor = mrcp_engine_sink_stream_codec_get(channel);

	if(!descriptor) {
		apt_log(ANEEX_LOG_MARK,APT_PRIO_WARNING,"Failed to Get Codec Descriptor " APT_SIDRES_FMT, MRCP_MESSAGE_SIDRES(request));
		response->start_line.status_code = MRCP_STATUS_CODE_METHOD_FAILED;
		return FALSE;
	}

	recog_channel->timers_started = TRUE;

	/* get recognizer header */
	recog_header = mrcp_resource_header_get(request);
	if(recog_header) {
		if(mrcp_resource_header_property_check(request,ANEEX_HEADER_START_INPUT_TIMERS) == TRUE) {
			recog_channel->timers_started = recog_header->start_input_timers;
		}
		if(mrcp_resource_header_property_check(request,ANEEX_HEADER_NO_INPUT_TIMEOUT) == TRUE) {
			mpf_activity_detector_noinput_timeout_set(recog_channel->detector,recog_header->no_input_timeout);
		}
		if(mrcp_resource_header_property_check(request,ANEEX_HEADER_SPEECH_COMPLETE_TIMEOUT) == TRUE) {
			mpf_activity_detector_silence_timeout_set(recog_channel->detector,recog_header->speech_complete_timeout);
		}
	}

	if(!recog_channel->audio_out) {
		const apt_dir_layout_t *dir_layout = channel->engine->dir_layout;
		audio_file_name = apr_psprintf(channel->pool,"utter-%dkHz-%s.pcm",
							descriptor->sampling_rate/1000,
							request->channel_id.session_id.buf);
		audio_file_path = apt_vardir_filepath_get(dir_layout,audio_file_name,channel->pool);
		if(audio_file_path) {
			apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Open Utterance Output File [%s] for Writing",audio_file_path);
			recog_channel->audio_out = fopen(audio_file_path,"wb");
			if(!recog_channel->audio_out) {
				apt_log(ANEEX_LOG_MARK,APT_PRIO_WARNING,"Failed to Open Utterance Output File [%s] for Writing",audio_file_path);
			}
		}
	}

	response->start_line.request_state = MRCP_REQUEST_STATE_INPROGRESS;
	/* send asynchronous response */
	mrcp_engine_channel_message_send(channel,response);
	recog_channel->recog_request = request;
	
	return TRUE;
}

/** Process STOP request */
static apt_bool_t aneex_recog_channel_stop(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	/* process STOP request */
	aneex_recog_channel_t *recog_channel = channel->method_obj;
	/* store STOP request, make sure there is no more activity and only then send the response */
	recog_channel->stop_response = response;
	return TRUE;
}

/** Process START-INPUT-TIMERS request */
static apt_bool_t aneex_recog_channel_timers_start(mrcp_engine_channel_t *channel, mrcp_message_t *request, mrcp_message_t *response)
{
	aneex_recog_channel_t *recog_channel = channel->method_obj;
	recog_channel->timers_started = TRUE;
	return mrcp_engine_channel_message_send(channel,response);
}

/** Dispatch MRCP request */
static apt_bool_t aneex_recog_channel_request_dispatch(mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
    //printf("DEBUG: Plugin: aneex_recog_channel_request_dispatch\n");
	apt_bool_t processed = FALSE;
	mrcp_message_t *response = mrcp_response_create(request,request->pool);
	switch(request->start_line.method_id) {
		case ANEEX_SET_PARAMS:
			break;
		case ANEEX_GET_PARAMS:
			break;
		case ANEEX_DEFINE_GRAMMAR:
			break;
		case ANEEX_RECOGNIZE:
			processed = aneex_recog_channel_recognize(channel,request,response);
			break;
		case ANEEX_GET_RESULT:
			break;
		case ANEEX_START_INPUT_TIMERS:
			processed = aneex_recog_channel_timers_start(channel,request,response);
			break;
		case ANEEX_STOP:
			processed = aneex_recog_channel_stop(channel,request,response);
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
static apt_bool_t aneex_recog_stream_destroy(mpf_audio_stream_t *stream)
{
	return TRUE;
}

/** Callback is called from MPF engine context to perform any action before open */
static apt_bool_t aneex_recog_stream_open(mpf_audio_stream_t *stream, mpf_codec_t *codec)
{
	return TRUE;
}

/** Callback is called from MPF engine context to perform any action after close */
static apt_bool_t aneex_recog_stream_close(mpf_audio_stream_t *stream)
{
	return TRUE;
}

/* Raise aneex START-OF-INPUT event */
static apt_bool_t aneex_recog_start_of_input(aneex_recog_channel_t *recog_channel)
{
    //printf("DEBUG: Plugin: demo_recog_start_of_input\n");
    pthread_mutex_init(&lock, NULL);

	/* create START-OF-INPUT event */
	mrcp_message_t *message = mrcp_event_create(
						recog_channel->recog_request,
						ANEEX_START_OF_INPUT,
						recog_channel->recog_request->pool);
	if(!message) {
		return FALSE;
	}

	/* set request state */
	message->start_line.request_state = MRCP_REQUEST_STATE_INPROGRESS;
	/* send asynch event */
	return mrcp_engine_channel_message_send(recog_channel->channel,message);
}

/* Load aneex recognition result */
static apt_bool_t aneex_recog_result_load(aneex_recog_channel_t *recog_channel, mrcp_message_t *message)
{
	printf("DEBUG: aneex_recog_result_load result\n");

	FILE *file;
	mrcp_engine_channel_t *channel = recog_channel->channel;
	const apt_dir_layout_t *dir_layout = channel->engine->dir_layout;
	char *file_path = apt_datadir_filepath_get(dir_layout,"result.xml",message->pool);
	if(!file_path) {
		return FALSE;
	}

	/* read the demo result from file */
	file = fopen(file_path,"r");
	if(file) {
		mrcp_generic_header_t *generic_header;
		char text[1024];
		apr_size_t size;
		size = fread(text,1,sizeof(text),file);
		apt_string_assign_n(&message->body,text,size,message->pool);
		fclose(file);

		generic_header = mrcp_generic_header_prepare(message);
		if(generic_header) {
			apt_string_assign(&generic_header->content_type,"application/x-nlsml",message->pool);
			mrcp_generic_header_property_add(message,GENERIC_HEADER_CONTENT_TYPE);
		}
	}

	return TRUE;
}

/* Raise aneex RECOGNITION-COMPLETE event */
static apt_bool_t aneex_recog_recognition_complete(aneex_recog_channel_t *recog_channel, aneex_recog_completion_cause_e cause)
{
	aneex_recog_header_t *recog_header;
	/* create RECOGNITION-COMPLETE event */
	mrcp_message_t *message = mrcp_event_create(
						recog_channel->recog_request,
						ANEEX_RECOGNITION_COMPLETE,
						recog_channel->recog_request->pool);
	if(!message) {
		return FALSE;
	}

	aneex_recog_from_db(audio_file_path, db_file_path, recog_channel);

	/* get/allocate recognizer header */
	recog_header = mrcp_resource_header_prepare(message);
	if(recog_header) {
		/* set completion cause */
		recog_header->completion_cause = cause;
		mrcp_resource_header_property_add(message,ANEEX_HEADER_COMPLETION_CAUSE);
	}
	/* set request state */
	message->start_line.request_state = MRCP_REQUEST_STATE_COMPLETE;

	if(cause == ANEEX_COMPLETION_CAUSE_SUCCESS) {
		aneex_recog_result_load(recog_channel,message);
	}

	recog_channel->recog_request = NULL;
	/* send asynch event */
	return mrcp_engine_channel_message_send(recog_channel->channel,message);
}

void* threadFunc(void* thread_data){
	//получаем структуру с данными
	pthrData* data = (pthrData*) thread_data;

	pthread_mutex_lock(&lock);
 	result=TestAneex(data->audio_path, data->db_path);
 	pthread_mutex_unlock(&lock);

	return NULL;
}

void aneex_recog_from_db(char *audio_path, char* db_path, aneex_recog_channel_t *recog_channel)
{
	if (result>0) {
		pthread_mutex_destroy(&lock);
		aneex_recog_recognition_complete(recog_channel,ANEEX_COMPLETION_CAUSE_SUCCESS);
	}
	else if (result==-1){
		pthread_mutex_destroy(&lock);
		aneex_recog_recognition_complete(recog_channel,ANEEX_COMPLETION_CAUSE_ERROR);
	}

	threadData.audio_path = audio_path;
	threadData.db_path = db_path;

	//запускаем поток
	pthread_create(&thread, NULL, threadFunc, &threadData);
	pthread_join(thread,NULL);

	//printf("Result from plugin=%d\n", result);
}

/** Callback is called from MPF engine context to write/send new frame */
static apt_bool_t aneex_recog_stream_write(mpf_audio_stream_t *stream, const mpf_frame_t *frame)
{
	//printf("DEBUG: aneex_recog_stream_write\n");

	aneex_recog_channel_t *recog_channel = stream->obj;
	if(recog_channel->stop_response) {
		/* send asynchronous response to STOP request */
		mrcp_engine_channel_message_send(recog_channel->channel,recog_channel->stop_response);
		recog_channel->stop_response = NULL;
		recog_channel->recog_request = NULL;
		return TRUE;
	}

	if(recog_channel->recog_request) {
		mpf_detector_event_e det_event = mpf_activity_detector_process(recog_channel->detector,frame);
		switch(det_event) {
			case MPF_DETECTOR_EVENT_ACTIVITY:
				apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Detected Voice Activity " APT_SIDRES_FMT,
					MRCP_MESSAGE_SIDRES(recog_channel->recog_request));
				aneex_recog_start_of_input(recog_channel);
				break;
			case MPF_DETECTOR_EVENT_INACTIVITY:
				apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Detected Voice Inactivity " APT_SIDRES_FMT,
					MRCP_MESSAGE_SIDRES(recog_channel->recog_request));
				aneex_recog_recognition_complete(recog_channel,ANEEX_COMPLETION_CAUSE_SUCCESS);
				break;
			case MPF_DETECTOR_EVENT_NOINPUT:
				apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Detected Noinput " APT_SIDRES_FMT,
					MRCP_MESSAGE_SIDRES(recog_channel->recog_request));
				if(recog_channel->timers_started == TRUE) {
					aneex_recog_recognition_complete(recog_channel,ANEEX_COMPLETION_CAUSE_NO_INPUT_TIMEOUT);
				}
				break;
			default:
				break;
		}

		if(recog_channel->recog_request) {
			if((frame->type & MEDIA_FRAME_TYPE_EVENT) == MEDIA_FRAME_TYPE_EVENT) {
				if(frame->marker == MPF_MARKER_START_OF_EVENT) {
					apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Detected Start of Event " APT_SIDRES_FMT " id:%d",
						MRCP_MESSAGE_SIDRES(recog_channel->recog_request),
						frame->event_frame.event_id);
				}
				else if(frame->marker == MPF_MARKER_END_OF_EVENT) {
					apt_log(ANEEX_LOG_MARK,APT_PRIO_INFO,"Detected End of Event " APT_SIDRES_FMT " id:%d duration:%d ts",
						MRCP_MESSAGE_SIDRES(recog_channel->recog_request),
						frame->event_frame.event_id,
						frame->event_frame.duration);
				}
			}
		}

		//записываем входяший голос сюда
		if(recog_channel->audio_out) {
			fwrite(frame->codec_frame.buffer,1,frame->codec_frame.size,recog_channel->audio_out);

			printf("File buffer=%d\n", ftell(recog_channel->audio_out));
			aneex_recog_from_db(audio_file_path, db_file_path, recog_channel);
		}

	}
	return TRUE;
}

static apt_bool_t aneex_recog_msg_signal(aneex_recog_msg_type_e type, mrcp_engine_channel_t *channel, mrcp_message_t *request)
{
	apt_bool_t status = FALSE;
	aneex_recog_channel_t *aneex_channel = channel->method_obj;
	aneex_recog_engine_t *aneex_engine = aneex_channel->aneex_engine;
	apt_task_t *task = apt_consumer_task_base_get(aneex_engine->task);
	apt_task_msg_t *msg = apt_task_msg_get(task);
	if(msg) {
		aneex_recog_msg_t *aneex_msg;
		msg->type = TASK_MSG_USER;
		aneex_msg = (aneex_recog_msg_t*) msg->data;

		aneex_msg->type = type;
		aneex_msg->channel = channel;
		aneex_msg->request = request;
		status = apt_task_msg_signal(task,msg);
	}
	return status;
}

static apt_bool_t aneex_recog_msg_process(apt_task_t *task, apt_task_msg_t *msg)
{
	printf("DEBUG: aneex_recog_msg_process\n");
	aneex_recog_msg_t *aneex_msg = (aneex_recog_msg_t*)msg->data;

	switch(aneex_msg->type) {
		case ANEEX_RECOG_MSG_OPEN_CHANNEL:
			/* open channel and send asynch response */
			mrcp_engine_channel_open_respond(aneex_msg->channel,TRUE);
			break;
		case ANEEX_RECOG_MSG_CLOSE_CHANNEL:
		{
			/* close channel, make sure there is no activity and send asynch response */
			aneex_recog_channel_t *recog_channel = aneex_msg->channel->method_obj;
			if(recog_channel->audio_out) {
				fclose(recog_channel->audio_out);
				recog_channel->audio_out = NULL;
			}

			mrcp_engine_channel_close_respond(aneex_msg->channel);
			break;
		}
		case ANEEX_RECOG_MSG_REQUEST_PROCESS:
			aneex_recog_channel_request_dispatch(aneex_msg->channel,aneex_msg->request);
			break;
		default:
			break;
	}
	return TRUE;
}
