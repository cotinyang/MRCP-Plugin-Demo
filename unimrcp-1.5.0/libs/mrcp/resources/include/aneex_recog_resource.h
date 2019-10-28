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

#ifndef ANEEX_RECOG_RESOURCE_H
#define ANEEX_RECOG_RESOURCE_H

/**
 * @file aneex_recog_resource.h
 * @brief Aneex Recognizer Resource
 */ 

#include "mrcp_types.h"

APT_BEGIN_EXTERN_C

/** MRCP recognizer methods */
typedef enum {
	ANEEX_SET_PARAMS,
	ANEEX_GET_PARAMS,
	ANEEX_DEFINE_GRAMMAR,
	ANEEX_RECOGNIZE,
	ANEEX_INTERPRET,
	ANEEX_GET_RESULT,
	ANEEX_START_INPUT_TIMERS,
	ANEEX_STOP,
	ANEEX_START_PHRASE_ENROLLMENT,
	ANEEX_ENROLLMENT_ROLLBACK,
	ANEEX_END_PHRASE_ENROLLMENT,
	ANEEX_MODIFY_PHRASE,
	ANEEX_DELETE_PHRASE,

	ANEEX_METHOD_COUNT
} aneex_recognizer_method_id;

/** MRCP recognizer events */
typedef enum {
	ANEEX_START_OF_INPUT,
	ANEEX_RECOGNITION_COMPLETE,
	ANEEX_INTERPRETATION_COMPLETE,

	ANEEX_EVENT_COUNT
} aneex_recognizer_event_id;

/** Create MRCP recognizer resource */
MRCP_DECLARE(mrcp_resource_t*) aneex_recog_resource_create(apr_pool_t *pool);

APT_END_EXTERN_C

#endif /* MRCP_RECOG_RESOURCE_H */
