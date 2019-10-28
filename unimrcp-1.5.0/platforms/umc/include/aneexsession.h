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

#ifndef ANEEXRECORDER_SESSION_H
#define ANEEXRECORDER_SESSION_H

/**
 * @file ANEEXsession.h
 * @brief ANEEXRecorder Session
 */ 

#include "umcsession.h"

class AneexRecorderScenario;
struct AneexRecorderChannel;

class AneexRecorderSession : public UmcSession
{
public:
/* ============================ CREATORS =================================== */
	AneexRecorderSession(const RecorderScenario* pScenario);
	virtual ~AneexRecorderSession();

protected:
/* ============================ MANIPULATORS =============================== */
	virtual bool Start();

	AneexRecorderChannel* CreateRecorderChannel();
	bool StartRecorder(mrcp_channel_t* pMrcpChannel);

	mrcp_message_t* CreateRecordRequest(mrcp_channel_t* pMrcpChannel);

	FILE* GetAudioIn(const mpf_codec_descriptor_t* pDescriptor, apr_pool_t* pool) const;

/* ============================ HANDLERS =================================== */
	virtual bool OnSessionTerminate(mrcp_sig_status_code_e status);
	virtual bool OnChannelAdd(mrcp_channel_t* pMrcpChannel, mrcp_sig_status_code_e status);
	virtual bool OnMessageReceive(mrcp_channel_t* pMrcpChannel, mrcp_message_t* pMrcpMessage);

/* ============================ ACCESSORS ================================== */
	const RecorderScenario* GetScenario() const;

private:
/* ============================ DATA ======================================= */
	RecorderChannel* m_pRecorderChannel;
};


/* ============================ INLINE METHODS ============================= */
inline const AneexRecorderScenario* AneexRecorderSession::GetScenario() const
{
	return (AneexRecorderScenario*)m_pScenario;
}

#endif /* RECORDER_SESSION_H */
