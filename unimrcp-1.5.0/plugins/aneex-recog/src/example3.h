/*
Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef IDENTIFICATIONTASK_H
#define IDENTIFICATIONTASK_H

#include <cstdint>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>

#include "IdTask.h"
#include "AudioSource.h"
#include "AudioBlock.h"

namespace bfs = boost::filesystem;

std::string filename;


class IdentificationTask : public IdTask
{
    AudioSourceFile                m_AudioSource;
    std::string                    m_Audiofile;
    bool                           m_Terminated;
    IdentificationResultsListener *m_Listener;


    void Identify(const std::string &audioclip)
    {
        if(m_Terminated) return;

        if(bfs::is_regular_file(audioclip))
        {
           filename = bfs::path(audioclip).filename().string();

           std::cout << "=========================================================" << std::endl;
           std::cout << "Identifying " << filename << " ..." << std::endl;

           // Read audio data from file

           const Audioneex::IdMatch* results = nullptr;

           AudioBlock<int16_t> iblock;
           AudioBlock<float>   iaudio;

           m_AudioSource.Open(audioclip);

           // process 1-2 second blocks from the audio source

           // create the input buffer
           iblock.Create(size_t(11025 * 1.2), 11025, 1);
           // create the normalized input audio block
           iaudio.Create(size_t(11025 * 1.2), 11025, 1);

           // Read audio blocks from the source and perform identification
           // until results are produced or all audio is exhausted
           do{
               m_AudioSource.GetAudioBlock(iblock);

               iblock.Normalize( iaudio );

               m_Recognizer->Identify(iaudio.Data(), iaudio.Size());
               results = m_Recognizer->GetResults();
           }
           while(iblock.Size() > 0 && !results);

           // The id engine will always produce results if enough audio
           // is provided for it to make a decision. If the audio data
           // is exhausted before the engine returns a results you may
           // flush the internal buffers and check again.
           if(!results){
              m_Recognizer->Flush();
              results = m_Recognizer->GetResults();
           }

           // Notify whoever wants to be notified about the results.
           // NOTE: The listeners should make a deep copy of the results
           // if they are used in asynchronous contexts as the pointer
           // will become invalid after resetting the recognizer.
           if(results && m_Listener)
              m_Listener->OnResults( results );
           else
              std::cout << "Not enough audio." << std::endl;

           // IMPORTANT:
           // Always reset the recognizer for new identifications if reused.
           m_Recognizer->Reset();

           m_AudioSource.Close();

        }
        else if(bfs::is_directory(audioclip))
        {
           bfs::directory_iterator it(audioclip);
           for(; it!=bfs::directory_iterator(); ++it)
               Identify(it->path().string());
        }
    }

public:

    explicit IdentificationTask(const std::string &audiofile) :
        m_Audiofile  (audiofile),
        m_Terminated (false),
        m_Listener   (nullptr)
    {
        m_AudioSource.SetSampleRate( 11025 );
        m_AudioSource.SetChannelCount( 1 );
        m_AudioSource.SetSampleResolution( 16 );
    }

    void Run()
    {
        assert(m_Recognizer);

        if(!bfs::exists(m_Audiofile))
           throw std::runtime_error("File not found " + m_Audiofile);

        Identify( m_Audiofile );
    }

    void Terminate()
    { 
        m_Terminated = true; 
    }

    void Connect(IdentificationResultsListener *listener)
    {
        m_Listener = listener;
    }

    AudioSource* GetAudioSource()
    { 
        return &m_AudioSource;
    }

};


/// ---------------------------------------------------------------------------

/// This class implements a parser for file identification results.
/// It is just a convenience class that connects to the identification
/// task to get the results and do something with them.
class FileIdentificationResultsParser : public IdentificationResultsListener
{
    std::shared_ptr<KVDataStore>           m_Datastore;
    std::shared_ptr<Audioneex::Recognizer> m_Recognizer;

  public:

    void OnResults(const Audioneex::IdMatch *results)
    {
        std::vector<Audioneex::IdMatch> BestMatch;

        // Get the best match(es), if any (there may be ties)
        if(results)
           for(int i=0; !Audioneex::IsNull(results[i]); i++)
               BestMatch.push_back( results[i] );

        // We have a single best match
        if(BestMatch.size() == 1)
        {
           assert(m_Datastore);
           assert(m_Recognizer);

           // Get metadata for the best match
           std::string meta = m_Datastore->GetMetadata(BestMatch[0].FID);

           // The audio was identified.
           if(BestMatch[0].IdClass == Audioneex::IDENTIFIED)
           {
               std::cout << "IDENTIFIED  FID: " << BestMatch[0].FID << std::endl;
               std::cout << "Score: " << BestMatch[0].Score << ", ";
               std::cout << "Conf.: " << BestMatch[0].Confidence << ", ";
               std::cout << (meta.empty() ? "No metadata" : meta) << std::endl;
           }

           // The audio have similarities with the found match, but not so strong
           // to call it identified. You can accept or ignore these matches.
           // NOTE: This result is only given in the fuzzy identification.
           else if(BestMatch[0].IdClass == Audioneex::SOUNDS_LIKE)
           {
               std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
               std::cout << "SOUNDS LIKE FID: " << BestMatch[0].FID << std::endl;
               std::cout << "Conf.: " << BestMatch[0].Confidence << std::endl;
               std::cout << (meta.empty() ? "No metadata" : meta) << std::endl;
           }
           // This should never happen
           else
               std::cout << "FATAL ERROR: Unexpected identification class" << std::endl;

        }
        // There are ties for the best match
        else if(BestMatch.size() > 1)
        {
            std::cout << "There are " << BestMatch.size() << " ties for the best match" << std::endl;
        }
        else
        {
            std::cout << "NO MATCH FOUND" << std::endl;
        }

        std::cout << "ID Time: " << m_Recognizer->GetIdentificationTime() << " s" << std::endl;
        std::cout << "=========================================================" << std::endl;
    }

    void SetDatastore(std::shared_ptr<KVDataStore> &store)
    {
        m_Datastore = store;
    }
	
    void SetRecognizer(std::shared_ptr<Audioneex::Recognizer> &recognizer)
    {
        m_Recognizer = recognizer;
    }
};

#endif
