/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef IDTASK_H
#define IDTASK_H


#include <cassert>
#include <memory>

#include "KVDataStore.h"
#include "AudioSource.h"
#include "audioneex.h"


/// Abstract class for various tasks performed by the identification
/// engine. Concrete implementations will provide task-specific functionality.

class IdTask
{
protected:

    std::shared_ptr<Audioneex::Indexer>     m_Indexer;
    std::shared_ptr<Audioneex::Recognizer>  m_Recognizer;
    std::shared_ptr<KVDataStore>            m_DataStore;

public:

    IdTask() = default;
    virtual ~IdTask() = default;

    virtual void Run() = 0;
    virtual void Terminate() = 0;

    virtual AudioSource* GetAudioSource() = 0;

    void SetIndexer(std::shared_ptr<Audioneex::Indexer> &indexer) 
    {
        m_Indexer = indexer;
    }

    void SetRecognizer(std::shared_ptr<Audioneex::Recognizer> &recognizer) 
    {
        m_Recognizer = recognizer;
    }

    void SetDataStore(std::shared_ptr<KVDataStore> &datastore) 
    {
        m_DataStore = datastore;
    }

};


#endif
