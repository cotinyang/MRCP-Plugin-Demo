/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef AUDIOINDEXINGTASK_H
#define AUDIOINDEXINGTASK_H

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <boost/filesystem.hpp>

#include "IdTask.h"
#include "AudioSource.h"
#include "AudioBlock.h"

namespace bfs = boost::filesystem;


class AudioIndexingTask : public IdTask,
                          public Audioneex::AudioProvider
{
    std::string         m_AudioDatabaseDir;
    uint32_t            m_FID;
    bool                m_Terminated;
    AudioSourceFile     m_AudioSource;
    AudioBlock<int16_t> m_InputBlock;
    AudioBlock<float>   m_AudioBlock;


    int OnAudioData(uint32_t FID, float *buffer, size_t nsamples)
    {
        assert(FID == m_FID);

        // Read 'nsamples' from the audio source into the indexer buffer.
        // 'nsamples' always amount to less then 10 seconds, so you can
        // safely allocate 10 second input buffers.

        // NOTE: Audio must be 16 bit normalized in [-1,1], mono, 11025 Hz.

        // Set the input buffer to the requested amount of samples.
        m_InputBlock.Resize( nsamples );

        m_AudioSource.GetAudioBlock( m_InputBlock );

        // If we reached the end of the stream, signal it to the indexer
        // by returning 0.
        if(m_InputBlock.Size() == 0)
           return 0;

        // The audio must be normalized prior to fingerprinting
        m_InputBlock.Normalize( m_AudioBlock );

        // Copy the audio into indexer buffer
        std::copy(m_AudioBlock.Data(),
                  m_AudioBlock.Data() + m_AudioBlock.Size(),
                  buffer);

        return m_AudioBlock.Size();
	}
	

    void DoIndexing(const std::string &audiofile)
    {
        if(m_Terminated) return;


	    if(bfs::is_regular_file(audiofile))
	    {
           std::string filename = bfs::path(audiofile).filename().string();
           std::string ext = filename.substr(filename.find_last_of(".")+1);

           if(AudioSource::IsFormatSupported(ext))
           {
              m_AudioSource.Open(audiofile);

              std::cout << "[FID:"<<(m_FID+1)<<"] - " << filename << " ... ";
              std::cout.flush();

              // Assign a strict increasing FID to the recordings.
              m_FID++;

              // You may want to catch indexing errors here for recoverable 
              // failures in order to avoid aborting the whole indexing process.
              // Invalid fingerprint exceptions are thrown if a fingerprint 
              // cannot be extracted for some reason, such as invalid audio or 
              // incorrect FIDs. These errors may be recovered by just skipping 
              // the failed fingerprint. Anything else is a serious error and 
              // indexing should be aborted.
              try{
                  // Start indexing of current audio file.
                  // This call is synchronous (i.e. blocking). It will call 
                  // OnAudioData() repeatedly and will return once all audio 
                  // for the current recording has been consumed (this must be 
                  // signalled in OnAudioData() by returning 0).
                  m_Indexer->Index(m_FID);

                  // Here, after the recording has been successfully indexed,
                  // you may want to store the FID and associated metadata in
                  // a database ...

                  std::string meta;

#ifdef WITH_ID3
                  if(m_AudioSource.GetID3Tags().GetTitle().empty() &&
                     m_AudioSource.GetID3Tags().GetArtist().empty())
                     meta = filename;
                  else
                     meta = m_AudioSource.GetID3Tags().GetTitle() + " by " +
                            m_AudioSource.GetID3Tags().GetArtist();

                  m_DataStore->PutMetadata( m_FID, meta );
#endif

				  std::cout << "OK. (" << m_AudioSource.GetFormattedDuration() 
                            << ")" << (meta.empty()?"":" (ID3)") << std::endl;
              }
              catch(const Audioneex::InvalidFingerprintException &ex)
              {
                  std::cerr << "FAILED. " << ex.what() << std::endl;
                  m_FID--; // reuse FID
              }

              // Reset all resources for next audio file
              m_AudioSource.Close();
           }
           else
              std::cout << "Unsupported format: " << filename << std::endl;
	    }
	    else if(bfs::is_directory(audiofile))
	    {
	       bfs::directory_iterator it(audiofile);
	       for(; it!=bfs::directory_iterator(); ++it)
	           DoIndexing(it->path().string());
	    }
	}

public:

    explicit AudioIndexingTask(const std::string &audio_db_dir) :
        m_AudioDatabaseDir  (audio_db_dir),
        m_InputBlock        (11025*10, 11025, 1), //< 10s buffer is enough
        m_AudioBlock        (11025*10, 11025, 1), //< the normalized audio
        m_FID               (0),
        m_Terminated        (false)
    {}

    void Run()
    {
        assert(m_Indexer);

        if(!bfs::exists( m_AudioDatabaseDir ))
           throw std::invalid_argument(m_AudioDatabaseDir+" does not exist");

        m_AudioSource.SetSampleRate( 11025 );
        m_AudioSource.SetChannelCount( 1 );
        m_AudioSource.SetSampleResolution( 16 );

        m_Indexer->Start();
        DoIndexing( m_AudioDatabaseDir );
        m_Indexer->End( !m_Terminated ); //< don't flush the cache if terminated
	}

    void Terminate()
    { 
        m_Terminated = true; 
    }
    
    void SetFID(uint32_t fid)
    { 
        m_FID = fid; 
    }
    
    AudioSource* GetAudioSource()
    { 
        return &m_AudioSource; 
    }

};


#endif
