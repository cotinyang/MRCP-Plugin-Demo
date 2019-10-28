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

#include "aneex_recog_header.h"

/** String table of MRCPv1 ANEEX header fields (mrcp_recog_header_id) */
static const apt_str_table_item_t v1_recog_header_string_table[] = {
	{{"Confidence-Threshold",             20},16},
	{{"Sensitivity-Level",                17},14},
	{{"Speed-Vs-Accuracy",                17},4},
	{{"N-Best-List-Length",               18},1},
	{{"No-Input-Timeout",                 16},2},
	{{"Recognition-Timeout",              19},19},
	{{"Waveform-Url",                     12},4},
	{{"Completion-Cause",                 16},16},
	{{"ANEEX-Context-Block",         24},16},
	{{"ANEEX-Start-Timers",          23},18},
	{{"Speech-Complete-Timeout",          23},7},
	{{"Speech-Incomplete-Timeout",        25},12},
	{{"DTMF-Interdigit-Timeout",          23},10},
	{{"DTMF-Term-Timeout",                17},14},
	{{"DTMF-Term-Char",                   14},14},
	{{"Failed-Uri",                       10},10},
	{{"Failed-Uri-Cause",                 16},16},
	{{"Save-Waveform",                    13},5},
	{{"New-Audio-Channel",                17},17},
	{{"Speech-Language",                  15},8},
	{{"Input-Type",                       10},10},
	{{"Input-Waveform-Uri",               18},6},
	{{"Completion-Reason",                17},17},
	{{"Media-Type",                       10},0},
	{{"Ver-Buffer-Utterance",             20},0},
	{{"Recognition-Mode",                 16},16},
	{{"Cancel-If-Queue",                  15},3},
	{{"Hotword-Max-Duration",             20},10},
	{{"Hotword-Min-Duration",             20},20},
	{{"Interpret-Text",                   14},12},
	{{"DTMF-Buffer-Time",                 16},16},
	{{"Clear-DTMF-Buffer",                17},11},
	{{"Early-No-Match",                   14},4},
	{{"Num-Min-Consistent-Pronunciations",33},1},
	{{"Consistency-Threshold",            21},16},
	{{"Clash-Threshold",                  15},2},
	{{"Personal-Grammar-URI",             20},9},
	{{"Enroll-Utterance",                 16},10},
	{{"Phrase-ID",                         9},8},
	{{"Phrase-NL",                         9},9},
	{{"Weight",                            6},3},
	{{"Save-Best-Waveform",               18},10},
	{{"New-Phrase-ID",                    13},4},
	{{"Confusable-Phrases-URI",           22},4},
	{{"Abort-Phrase-Enrollment",          23},0}
};

/** String table of MRCPv2 ANEEX header fields (mrcp_recog_header_id) */
static const apt_str_table_item_t v2_recog_header_string_table[] = {
	{{"Confidence-Threshold",             20},16},
	{{"Sensitivity-Level",                17},14},
	{{"Speed-Vs-Accuracy",                17},4},
	{{"N-Best-List-Length",               18},1},
	{{"No-Input-Timeout",                 16},2},
	{{"Recognition-Timeout",              19},19},
	{{"Waveform-Uri",                     12},4},
	{{"Completion-Cause",                 16},16},
	{{"ANEEX-Context-Block",         24},7},
	{{"Start-Input-Timers",               18},18},
	{{"Speech-Complete-Timeout",          23},7},
	{{"Speech-Incomplete-Timeout",        25},12},
	{{"DTMF-Interdigit-Timeout",          23},10},
	{{"DTMF-Term-Timeout",                17},14},
	{{"DTMF-Term-Char",                   14},14},
	{{"Failed-Uri",                       10},10},
	{{"Failed-Uri-Cause",                 16},16},
	{{"Save-Waveform",                    13},5},
	{{"New-Audio-Channel",                17},17},
	{{"Speech-Language",                  15},8},
	{{"Input-Type",                       10},10},
	{{"Input-Waveform-Uri",               18},6},
	{{"Completion-Reason",                17},13},
	{{"Media-Type",                       10},0},
	{{"Ver-Buffer-Utterance",             20},0},
	{{"Recognition-Mode",                 16},16},
	{{"Cancel-If-Queue",                  15},3},
	{{"Hotword-Max-Duration",             20},10},
	{{"Hotword-Min-Duration",             20},20},
	{{"Interpret-Text",                   14},12},
	{{"DTMF-Buffer-Time",                 16},16},
	{{"Clear-DTMF-Buffer",                17},11},
	{{"Early-No-Match",                   14},4},
	{{"Num-Min-Consistent-Pronunciations",33},1},
	{{"Consistency-Threshold",            21},16},
	{{"Clash-Threshold",                  15},15},
	{{"Personal-Grammar-URI",             20},9},
	{{"Enroll-Utterance",                 16},10},
	{{"Phrase-ID",                         9},8},
	{{"Phrase-NL",                         9},9},
	{{"Weight",                            6},3},
	{{"Save-Best-Waveform",               18},10},
	{{"New-Phrase-ID",                    13},4},
	{{"Confusable-Phrases-URI",           22},4},
	{{"Abort-Phrase-Enrollment",          23},0}
};

/** String table of MRCPv1 ANEEX completion-cause fields (mrcp_recog_completion_cause_e) */
static const apt_str_table_item_t v1_completion_cause_string_table[] = {
	{{"success",                     7},1},
	{{"no-match",                    8},8},
	{{"no-input-timeout",           16},3},
	{{"recognition-timeout",        19},0},
	{{"gram-load-failure",          17},7},
	{{"gram-comp-failure",          17},5},
	{{"error",                       5},0},
	{{"speech-too-early",           16},1},
	{{"too-much-speech-timeout",    23},0},
	{{"uri-failure",                11},0},
	{{"language-unsupported",       20},0},
	{{"cancelled",                   9},0},
	{{"semantics-failure",          17},2},
	{{"partial-match",              13},13},
	{{"partial-match-maxtime",      21},13},
	{{"no-match-maxtime",           16},9},
	{{"gram-definition-failure",    23},5}
};


/** String table of MRCPv2 ANEEX completion-cause fields (mrcp_recog_completion_cause_e) */
static const apt_str_table_item_t v2_completion_cause_string_table[] = {
	{{"success",                     7},7},
	{{"no-match",                    8},4},
	{{"no-input-timeout",           16},3},
	{{"hotword-maxtime",            15},0},
	{{"grammar-load-failure",       20},8},
	{{"grammar-compilation-failure",27},8},
	{{"ANEEX-error",           16},0},
	{{"speech-too-early",           16},1},
	{{"success-maxtime",            15},15},
	{{"uri-failure",                11},0},
	{{"language-unsupported",       20},0},
	{{"cancelled",                   9},0},
	{{"semantics-failure",          17},2},
	{{"partial-match",              13},13},
	{{"partial-match-maxtime",      21},13},
	{{"no-match-maxtime",           16},9},
	{{"grammar-definition-failure", 26},9}
};

/** Initialize ANEEX header */
static void aneex_recog_header_init(aneex_recog_header_t *recog_header)
{
	recog_header->confidence_threshold = 0.0;
	recog_header->sensitivity_level = 0.0;
	recog_header->speed_vs_accuracy = 0.0;
	recog_header->n_best_list_length = 0;
	recog_header->no_input_timeout = 0;
	recog_header->recognition_timeout = 0;
	apt_string_reset(&recog_header->waveform_uri);
	recog_header->completion_cause = ANEEX_COMPLETION_CAUSE_COUNT;
	apt_string_reset(&recog_header->ANEEX_context_block);
	recog_header->start_input_timers = FALSE;
	recog_header->speech_complete_timeout = 0;
	recog_header->speech_incomplete_timeout = 0;
	recog_header->dtmf_interdigit_timeout = 0;
	recog_header->dtmf_term_timeout = 0;
	recog_header->dtmf_term_char = 0;
	apt_string_reset(&recog_header->failed_uri);
	apt_string_reset(&recog_header->failed_uri_cause);
	recog_header->save_waveform = FALSE;
	recog_header->new_audio_channel = FALSE;
	apt_string_reset(&recog_header->speech_language);
	/* initializes additionnal MRCPV2 recog header fields */
	apt_string_reset(&recog_header->input_type);
	apt_string_reset(&recog_header->input_waveform_uri);
	apt_string_reset(&recog_header->completion_reason);
	apt_string_reset(&recog_header->media_type);
	recog_header->ver_buffer_utterance = FALSE;
	apt_string_reset(&recog_header->recognition_mode);
	recog_header->cancel_if_queue = FALSE;
	recog_header->hotword_max_duration = 0;
	recog_header->hotword_min_duration = 0;
	apt_string_reset(&recog_header->interpret_text);
	recog_header->dtmf_buffer_time = 0;
	recog_header->clear_dtmf_buffer = FALSE;
	recog_header->early_no_match = FALSE;
	recog_header->num_min_consistent_pronunciations = 0;
	recog_header->consistency_threshold = 0.0;
	recog_header->clash_threshold = 0.0;
	apt_string_reset(&recog_header->personal_grammar_uri);
	recog_header->enroll_utterance = FALSE;
	apt_string_reset(&recog_header->phrase_id);
	apt_string_reset(&recog_header->phrase_nl);
	recog_header->weight = 0.0;
	recog_header->save_best_waveform = FALSE;
	apt_string_reset(&recog_header->new_phrase_id);
	apt_string_reset(&recog_header->confusable_phrases_uri);
	recog_header->abort_phrase_enrollment = FALSE;
}

/** Allocate MRCP ANEEX header */
static void* aneex_recog_header_allocate(mrcp_header_accessor_t *accessor, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = apr_palloc(pool,sizeof(aneex_recog_header_t));
	aneex_recog_header_init(recog_header);
	accessor->data = recog_header;
	return accessor->data;
}

/** Parse MRCP ANEEX header */
static apt_bool_t aneex_recog_header_parse(aneex_recog_header_t *recog_header, apr_size_t id, const apt_str_t *value, apr_pool_t *pool)
{
	apt_bool_t status = TRUE;
	switch(id) {
		case ANEEX_HEADER_N_BEST_LIST_LENGTH:
			recog_header->n_best_list_length = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_NO_INPUT_TIMEOUT:
			recog_header->no_input_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_RECOGNITION_TIMEOUT:
			recog_header->recognition_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_WAVEFORM_URI:
			recog_header->waveform_uri = *value;
			break;
		case ANEEX_HEADER_COMPLETION_CAUSE:
			recog_header->completion_cause = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_ANEEX_CONTEXT_BLOCK:
			recog_header->ANEEX_context_block = *value;
			break;
		case ANEEX_HEADER_START_INPUT_TIMERS:
			apt_boolean_value_parse(value,&recog_header->start_input_timers);
			break;
		case ANEEX_HEADER_SPEECH_COMPLETE_TIMEOUT:
			recog_header->speech_complete_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_SPEECH_INCOMPLETE_TIMEOUT:
			recog_header->speech_incomplete_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_DTMF_INTERDIGIT_TIMEOUT:
			recog_header->dtmf_interdigit_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_DTMF_TERM_TIMEOUT:
			recog_header->dtmf_term_timeout = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_DTMF_TERM_CHAR:
			recog_header->dtmf_term_char = *value->buf;
			break;
		case ANEEX_HEADER_FAILED_URI:
			recog_header->failed_uri = *value;
			break;
		case ANEEX_HEADER_FAILED_URI_CAUSE:
			recog_header->failed_uri_cause = *value;
			break;
		case ANEEX_HEADER_SAVE_WAVEFORM:
			apt_boolean_value_parse(value,&recog_header->save_waveform);
			break;
		case ANEEX_HEADER_NEW_AUDIO_CHANNEL:
			apt_boolean_value_parse(value,&recog_header->new_audio_channel);
			break;
		case ANEEX_HEADER_SPEECH_LANGUAGE:
			recog_header->speech_language = *value;
			break;
		case ANEEX_HEADER_INPUT_TYPE:
			recog_header->input_type = *value;
			break;
		case ANEEX_HEADER_MEDIA_TYPE:
			recog_header->media_type = *value;
			break;
		case ANEEX_HEADER_INPUT_WAVEFORM_URI:
			recog_header->input_waveform_uri = *value;
			break;
		case ANEEX_HEADER_COMPLETION_REASON:
			recog_header->completion_reason = *value;
			break;
		case ANEEX_HEADER_VER_BUFFER_UTTERANCE:
			apt_boolean_value_parse(value,&recog_header->ver_buffer_utterance);
			break;
		case ANEEX_HEADER_RECOGNITION_MODE:
			recog_header->recognition_mode = *value;
			break;
		case ANEEX_HEADER_CANCEL_IF_QUEUE:
			apt_boolean_value_parse(value,&recog_header->cancel_if_queue);
			break;
		case ANEEX_HEADER_HOTWORD_MAX_DURATION:
			recog_header->hotword_max_duration = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_HOTWORD_MIN_DURATION:
			recog_header->hotword_min_duration = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_INTERPRET_TEXT:
			recog_header->interpret_text = *value;
			break;
		case ANEEX_HEADER_DTMF_BUFFER_TIME:
			recog_header->dtmf_buffer_time = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_CLEAR_DTMF_BUFFER:
			apt_boolean_value_parse(value,&recog_header->clear_dtmf_buffer);
			break;
		case ANEEX_HEADER_EARLY_NO_MATCH:
			apt_boolean_value_parse(value,&recog_header->early_no_match);
			break;
		case ANEEX_HEADER_NUM_MIN_CONSISTENT_PRONUNCIATIONS:
			recog_header->num_min_consistent_pronunciations = apt_size_value_parse(value);
			break;
		case ANEEX_HEADER_CONSISTENCY_THRESHOLD:
			recog_header->consistency_threshold = apt_float_value_parse(value);
			break;
		case ANEEX_HEADER_CLASH_THRESHOLD:
			recog_header->clash_threshold = apt_float_value_parse(value);
			break;
		case ANEEX_HEADER_PERSONAL_GRAMMAR_URI:
			recog_header->personal_grammar_uri = *value;
			break;
		case ANEEX_HEADER_ENROLL_UTTERANCE:
			apt_boolean_value_parse(value,&recog_header->enroll_utterance);
			break;
		case ANEEX_HEADER_PHRASE_ID:
			recog_header->phrase_id = *value;
			break;
		case ANEEX_HEADER_PHRASE_NL:
			recog_header->phrase_nl = *value;
			break;
		case ANEEX_HEADER_WEIGHT:
			recog_header->weight = apt_float_value_parse(value);
			break;
		case ANEEX_HEADER_SAVE_BEST_WAVEFORM:
			apt_boolean_value_parse(value,&recog_header->save_best_waveform);
			break;
		case ANEEX_HEADER_NEW_PHRASE_ID:
			recog_header->new_phrase_id = *value;
			break;
		case ANEEX_HEADER_CONFUSABLE_PHRASES_URI:
			recog_header->confusable_phrases_uri = *value;
			break;
		case ANEEX_HEADER_ABORT_PHRASE_ENROLLMENT:
			apt_boolean_value_parse(value,&recog_header->abort_phrase_enrollment);
			break;
		default:
			status = FALSE;
	}
	return status;
}

static APR_INLINE float apt_size_value_parse_as_float(const apt_str_t *value)
{
	float f = (float)apt_size_value_parse(value);
	return f / 100;
}

static APR_INLINE apt_bool_t apt_size_value_generate_from_float(float value, apt_str_t *str, apr_pool_t *pool)
{
	apr_size_t s = (apr_size_t)((value + 0.001f) * 100);
	return apt_size_value_generate(s,str,pool);
}

/** Parse MRCPv1 ANEEX header */
static apt_bool_t aneex_v1_recog_header_parse(mrcp_header_accessor_t *accessor, apr_size_t id, const apt_str_t *value, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = accessor->data;
	if(id == ANEEX_HEADER_CONFIDENCE_THRESHOLD) {
		recog_header->confidence_threshold = apt_size_value_parse_as_float(value);
		return TRUE;
	}
	else if(id == ANEEX_HEADER_SENSITIVITY_LEVEL) {
		recog_header->sensitivity_level = apt_size_value_parse_as_float(value);
		return TRUE;
	}
	else if(id == ANEEX_HEADER_SPEED_VS_ACCURACY) {
		recog_header->speed_vs_accuracy = apt_size_value_parse_as_float(value);
		return TRUE;
	}
	return aneex_recog_header_parse(recog_header,id,value,pool);
}

/** Parse MRCPv2 ANEEX header */
static apt_bool_t aneex_v2_recog_header_parse(mrcp_header_accessor_t *accessor, apr_size_t id, const apt_str_t *value, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = accessor->data;
	if(id == ANEEX_HEADER_CONFIDENCE_THRESHOLD) {
		recog_header->confidence_threshold = apt_float_value_parse(value);
		return TRUE;
	}
	else if(id == ANEEX_HEADER_SENSITIVITY_LEVEL) {
		recog_header->sensitivity_level = apt_float_value_parse(value);
		return TRUE;
	}
	else if(id == ANEEX_HEADER_SPEED_VS_ACCURACY) {
		recog_header->speed_vs_accuracy = apt_float_value_parse(value);
		return TRUE;
	}
	return aneex_recog_header_parse(recog_header,id,value,pool);
}

/** Generate MRCP ANEEX header */
static apt_bool_t aneex_recog_header_generate(const aneex_recog_header_t *recog_header, apr_size_t id, apt_str_t *value, apr_pool_t *pool)
{
	switch(id) {
		case ANEEX_HEADER_N_BEST_LIST_LENGTH:
			apt_size_value_generate(recog_header->n_best_list_length,value,pool);
			break;
		case ANEEX_HEADER_NO_INPUT_TIMEOUT:
			apt_size_value_generate(recog_header->no_input_timeout,value,pool);
			break;
		case ANEEX_HEADER_RECOGNITION_TIMEOUT:
			apt_size_value_generate(recog_header->recognition_timeout,value,pool);
			break;
		case ANEEX_HEADER_WAVEFORM_URI:
			*value = recog_header->waveform_uri;
			break;
		case ANEEX_HEADER_ANEEX_CONTEXT_BLOCK:
			*value = recog_header->ANEEX_context_block;
			break;
		case ANEEX_HEADER_START_INPUT_TIMERS:
			apt_boolean_value_generate(recog_header->start_input_timers,value,pool);
			break;
		case ANEEX_HEADER_SPEECH_COMPLETE_TIMEOUT:
			apt_size_value_generate(recog_header->speech_complete_timeout,value,pool);
			break;
		case ANEEX_HEADER_SPEECH_INCOMPLETE_TIMEOUT:
			apt_size_value_generate(recog_header->speech_incomplete_timeout,value,pool);
			break;
		case ANEEX_HEADER_DTMF_INTERDIGIT_TIMEOUT:
			apt_size_value_generate(recog_header->dtmf_interdigit_timeout,value,pool);
			break;
		case ANEEX_HEADER_DTMF_TERM_TIMEOUT:
			apt_size_value_generate(recog_header->dtmf_term_timeout,value,pool);
			break;
		case ANEEX_HEADER_DTMF_TERM_CHAR:
			value->length = 1;
			value->buf = apr_palloc(pool,value->length);
			*value->buf	= recog_header->dtmf_term_char;
			break;
		case ANEEX_HEADER_FAILED_URI:
			*value = recog_header->failed_uri;
			break;
		case ANEEX_HEADER_FAILED_URI_CAUSE:
			*value = recog_header->failed_uri_cause;
			break;
		case ANEEX_HEADER_SAVE_WAVEFORM:
			apt_boolean_value_generate(recog_header->save_waveform,value,pool);
			break;
		case ANEEX_HEADER_NEW_AUDIO_CHANNEL:
			apt_boolean_value_generate(recog_header->new_audio_channel,value,pool);
			break;
		case ANEEX_HEADER_SPEECH_LANGUAGE:
			*value = recog_header->speech_language;
			break;
		case ANEEX_HEADER_INPUT_TYPE:
			*value = recog_header->input_type;
			break;
		case ANEEX_HEADER_INPUT_WAVEFORM_URI:
			*value = recog_header->input_waveform_uri;
			break;
		case ANEEX_HEADER_COMPLETION_REASON:
			*value = recog_header->completion_reason;
			break;
		case ANEEX_HEADER_MEDIA_TYPE:
			*value = recog_header->media_type;
			break;
		case ANEEX_HEADER_VER_BUFFER_UTTERANCE:
			apt_boolean_value_generate(recog_header->ver_buffer_utterance,value,pool);
			break;
		case ANEEX_HEADER_RECOGNITION_MODE:
			*value = recog_header->recognition_mode;
			break;
		case ANEEX_HEADER_CANCEL_IF_QUEUE:
			apt_boolean_value_generate(recog_header->cancel_if_queue,value,pool);
			break;
		case ANEEX_HEADER_HOTWORD_MAX_DURATION:
			apt_size_value_generate(recog_header->hotword_max_duration,value,pool);
			break;
		case ANEEX_HEADER_HOTWORD_MIN_DURATION:
			apt_size_value_generate(recog_header->hotword_min_duration,value,pool);
			break;
		case ANEEX_HEADER_INTERPRET_TEXT:
			*value = recog_header->interpret_text;
			break;
		case ANEEX_HEADER_DTMF_BUFFER_TIME:
			apt_size_value_generate(recog_header->dtmf_buffer_time,value,pool);
			break;
		case ANEEX_HEADER_CLEAR_DTMF_BUFFER:
			apt_boolean_value_generate(recog_header->clear_dtmf_buffer,value,pool);
			break;
		case ANEEX_HEADER_EARLY_NO_MATCH:
			apt_boolean_value_generate(recog_header->early_no_match,value,pool);
			break;
		case ANEEX_HEADER_NUM_MIN_CONSISTENT_PRONUNCIATIONS:
			apt_size_value_generate(recog_header->num_min_consistent_pronunciations,value,pool);
			break;
		case ANEEX_HEADER_CONSISTENCY_THRESHOLD:
			apt_float_value_generate(recog_header->consistency_threshold,value,pool);
			break;
		case ANEEX_HEADER_CLASH_THRESHOLD:
			apt_float_value_generate(recog_header->clash_threshold,value,pool);
			break;
		case ANEEX_HEADER_PERSONAL_GRAMMAR_URI:
			*value = recog_header->personal_grammar_uri;
			break;
		case ANEEX_HEADER_ENROLL_UTTERANCE:
			apt_boolean_value_generate(recog_header->enroll_utterance,value,pool);
			break;
		case ANEEX_HEADER_PHRASE_ID:
			*value = recog_header->phrase_id;
			break;
		case ANEEX_HEADER_PHRASE_NL:
			*value = recog_header->phrase_nl;
			break;
		case ANEEX_HEADER_WEIGHT:
			apt_float_value_generate(recog_header->weight,value,pool);
			break;
		case ANEEX_HEADER_SAVE_BEST_WAVEFORM:
			apt_boolean_value_generate(recog_header->save_best_waveform,value,pool);
			break;
		case ANEEX_HEADER_NEW_PHRASE_ID:
			*value = recog_header->new_phrase_id;
			break;
		case ANEEX_HEADER_CONFUSABLE_PHRASES_URI:
			*value = recog_header->confusable_phrases_uri;
			break;
		case ANEEX_HEADER_ABORT_PHRASE_ENROLLMENT:
			apt_boolean_value_generate(recog_header->abort_phrase_enrollment,value,pool);
			break;
		default:
			break;
	}
	return TRUE;
}

/** Generate MRCPv1 ANEEX header */
static apt_bool_t aneex_v1_recog_header_generate(const mrcp_header_accessor_t *accessor, apr_size_t id, apt_str_t *value, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = accessor->data;
	if(id == ANEEX_HEADER_CONFIDENCE_THRESHOLD) {
		return apt_size_value_generate_from_float(recog_header->confidence_threshold,value,pool);
	}
	else if(id == ANEEX_HEADER_SENSITIVITY_LEVEL) {
		return apt_size_value_generate_from_float(recog_header->sensitivity_level,value,pool);
	}
	else if(id == ANEEX_HEADER_SPEED_VS_ACCURACY) {
		return apt_size_value_generate_from_float(recog_header->speed_vs_accuracy,value,pool);
	}
	else if(id == ANEEX_HEADER_COMPLETION_CAUSE) {
		return apt_completion_cause_generate(
			v1_completion_cause_string_table,
			ANEEX_COMPLETION_CAUSE_COUNT,
			recog_header->completion_cause,
			value,
			pool);
	}
	return aneex_recog_header_generate(recog_header,id,value,pool);
}

/** Generate MRCPv2 ANEEX header */
static apt_bool_t aneex_v2_recog_header_generate(const mrcp_header_accessor_t *accessor, apr_size_t id, apt_str_t *value, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = accessor->data;
	if(id == ANEEX_HEADER_CONFIDENCE_THRESHOLD) {
		return apt_float_value_generate(recog_header->confidence_threshold,value,pool);
	}
	else if(id == ANEEX_HEADER_SENSITIVITY_LEVEL) {
		return apt_float_value_generate(recog_header->sensitivity_level,value,pool);
	}
	else if(id == ANEEX_HEADER_SPEED_VS_ACCURACY) {
		return apt_float_value_generate(recog_header->speed_vs_accuracy,value,pool);
	}
	else if(id == ANEEX_HEADER_COMPLETION_CAUSE) {
		return apt_completion_cause_generate(
			v2_completion_cause_string_table,
			ANEEX_COMPLETION_CAUSE_COUNT,
			recog_header->completion_cause,
			value,
			pool);
	}
	return aneex_recog_header_generate(recog_header,id,value,pool);
}

/** Duplicate MRCP ANEEX header */
static apt_bool_t aneex_recog_header_duplicate(mrcp_header_accessor_t *accessor, const mrcp_header_accessor_t *src, apr_size_t id, const apt_str_t *value, apr_pool_t *pool)
{
	aneex_recog_header_t *recog_header = accessor->data;
	const aneex_recog_header_t *src_recog_header = src->data;
	apt_bool_t status = TRUE;

	if(!recog_header || !src_recog_header) {
		return FALSE;
	}
	
	switch(id) {
		case ANEEX_HEADER_CONFIDENCE_THRESHOLD:
			recog_header->confidence_threshold = src_recog_header->confidence_threshold;
			break;
		case ANEEX_HEADER_SENSITIVITY_LEVEL:
			recog_header->sensitivity_level = src_recog_header->sensitivity_level;
			break;
		case ANEEX_HEADER_SPEED_VS_ACCURACY:
			recog_header->speed_vs_accuracy = src_recog_header->speed_vs_accuracy;
			break;
		case ANEEX_HEADER_N_BEST_LIST_LENGTH:
			recog_header->n_best_list_length = src_recog_header->n_best_list_length;
			break;
		case ANEEX_HEADER_NO_INPUT_TIMEOUT:
			recog_header->no_input_timeout = src_recog_header->no_input_timeout;
			break;
		case ANEEX_HEADER_RECOGNITION_TIMEOUT:
			recog_header->recognition_timeout = src_recog_header->recognition_timeout;
			break;
		case ANEEX_HEADER_WAVEFORM_URI:
			recog_header->waveform_uri = *value;
			break;
		case ANEEX_HEADER_COMPLETION_CAUSE:
			recog_header->completion_cause = src_recog_header->completion_cause;
			break;
		case ANEEX_HEADER_ANEEX_CONTEXT_BLOCK:
			recog_header->ANEEX_context_block = *value;
			break;
		case ANEEX_HEADER_START_INPUT_TIMERS:
			recog_header->start_input_timers = src_recog_header->start_input_timers;
			break;
		case ANEEXHEADER_SPEECH_COMPLETE_TIMEOUT:
			recog_header->speech_complete_timeout = src_recog_header->speech_complete_timeout;
			break;
		case ANEEX_HEADER_SPEECH_INCOMPLETE_TIMEOUT:
			recog_header->speech_incomplete_timeout = src_recog_header->speech_incomplete_timeout;
			break;
		case ANEEX_HEADER_DTMF_INTERDIGIT_TIMEOUT:
			recog_header->dtmf_interdigit_timeout = src_recog_header->dtmf_interdigit_timeout;
			break;
		case ANEEX_HEADER_DTMF_TERM_TIMEOUT:
			recog_header->dtmf_term_timeout = src_recog_header->dtmf_term_timeout;
			break;
		case ANEEX_HEADER_DTMF_TERM_CHAR:
			recog_header->dtmf_term_char = src_recog_header->dtmf_term_char;
			break;
		case ANEEX_HEADER_FAILED_URI:
			recog_header->failed_uri = *value;
			break;
		case ANEEX_HEADER_FAILED_URI_CAUSE:
			recog_header->failed_uri_cause = *value;
			break;
		case ANEEX_HEADER_SAVE_WAVEFORM:
			recog_header->save_waveform = src_recog_header->save_waveform;
			break;
		case ANEEX_HEADER_NEW_AUDIO_CHANNEL:
			recog_header->new_audio_channel = src_recog_header->new_audio_channel;
			break;
		case ANEEX_HEADER_SPEECH_LANGUAGE:
			recog_header->speech_language = *value;
			break;
		case ANEEX_HEADER_INPUT_TYPE:
			recog_header->input_type = *value;
			break;
		case ANEEX_HEADER_INPUT_WAVEFORM_URI:
			recog_header->input_waveform_uri = *value;
			break;
		case ANEEX_HEADER_COMPLETION_REASON:
			recog_header->completion_reason = *value;
			break;
		case ANEEX_HEADER_MEDIA_TYPE:
			recog_header->media_type = *value;
			break;
		case ANEEX_HEADER_VER_BUFFER_UTTERANCE:
			recog_header->ver_buffer_utterance = src_recog_header->ver_buffer_utterance;
			break;
		case ANEEX_HEADER_RECOGNITION_MODE:
			recog_header->recognition_mode = *value;
			break;
		case ANEEX_HEADER_CANCEL_IF_QUEUE:
			recog_header->cancel_if_queue = src_recog_header->cancel_if_queue;
			break;
		case ANEEX_HEADER_HOTWORD_MAX_DURATION:
			recog_header->hotword_max_duration = src_recog_header->hotword_max_duration;
			break;
		case ANEEX_HEADER_HOTWORD_MIN_DURATION:
			recog_header->hotword_min_duration = src_recog_header->hotword_min_duration;
			break;
		case ANEEX_HEADER_INTERPRET_TEXT:
			recog_header->interpret_text = *value;
			break;
		case ANEEX_HEADER_DTMF_BUFFER_TIME:
			recog_header->dtmf_buffer_time = src_recog_header->dtmf_buffer_time;
			break;
		case ANEEX_HEADER_CLEAR_DTMF_BUFFER:
			recog_header->clear_dtmf_buffer = src_recog_header->clear_dtmf_buffer;
			break;
		case ANEEX_HEADER_EARLY_NO_MATCH:
			recog_header->early_no_match = src_recog_header->early_no_match;
			break;
		case ANEEX_HEADER_NUM_MIN_CONSISTENT_PRONUNCIATIONS:
			recog_header->num_min_consistent_pronunciations = src_recog_header->num_min_consistent_pronunciations;
			break;
		case ANEEX_HEADER_CONSISTENCY_THRESHOLD:
			recog_header->consistency_threshold = src_recog_header->consistency_threshold;
			break;
		case ANEEX_HEADER_CLASH_THRESHOLD:
			recog_header->clash_threshold = src_recog_header->clash_threshold;
			break;
		case ANEEX_HEADER_PERSONAL_GRAMMAR_URI:
			recog_header->personal_grammar_uri = *value;
			break;
		case ANEEX_HEADER_ENROLL_UTTERANCE:
			recog_header->enroll_utterance = src_recog_header->enroll_utterance;
			break;
		case ANEEX_HEADER_PHRASE_ID:
			recog_header->phrase_id = *value;
			break;
		case ANEEX_HEADER_PHRASE_NL:
			recog_header->phrase_nl = *value;
			break;
		case ANEEX_HEADER_WEIGHT:
			recog_header->weight = src_recog_header->weight;
			break;
		case ANEEX_HEADER_SAVE_BEST_WAVEFORM:
			recog_header->save_best_waveform = src_recog_header->save_best_waveform;
			break;
		case ANEEX_HEADER_NEW_PHRASE_ID:
			recog_header->new_phrase_id = *value;
			break;
		case ANEEX_HEADER_CONFUSABLE_PHRASES_URI:
			recog_header->confusable_phrases_uri = *value;
			break;
		case ANEEX_HEADER_ABORT_PHRASE_ENROLLMENT:
			recog_header->abort_phrase_enrollment = src_recog_header->abort_phrase_enrollment;
			break;
		default:
			status = FALSE;
	}
	return status;
}

static const mrcp_header_vtable_t v1_vtable = {
	aneex_recog_header_allocate,
	NULL, /* nothing to destroy */
	aneex_v1_recog_header_parse,
	aneex_v1_recog_header_generate,
	aneex_recog_header_duplicate,
	aneex_v1_recog_header_string_table,
	ANEEX_HEADER_COUNT
};

static const mrcp_header_vtable_t v2_vtable = {
	aneex_recog_header_allocate,
	NULL, /* nothing to destroy */
	aneex_v2_recog_header_parse,
	aneex_v2_recog_header_generate,
	aneex_recog_header_duplicate,
	aneex_v2_recog_header_string_table,
	ANEEX_HEADER_COUNT
};

const mrcp_header_vtable_t* aneex_recog_header_vtable_get(mrcp_version_e version)
{
	if(version == MRCP_VERSION_1) {
		return &v1_vtable;
	}
	return &v2_vtable;
}

MRCP_DECLARE(const apt_str_t*) aneex_recog_completion_cause_get(aneex_recog_completion_cause_e completion_cause, mrcp_version_e version)
{
	const apt_str_table_item_t *table = v2_completion_cause_string_table;
	if(version == MRCP_VERSION_1) {
		table = v1_completion_cause_string_table;
	}

	return apt_string_table_str_get(table,ANEEX_COMPLETION_CAUSE_COUNT,completion_cause);
}
