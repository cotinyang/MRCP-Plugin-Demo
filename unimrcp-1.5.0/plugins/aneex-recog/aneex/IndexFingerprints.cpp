/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

///
/// IndexFingerprints
///
/// This example shows how to perform the fingerprinting and indexing of audio 
/// from files. Different processing jobs performed by the identification 
/// engine are implemented into "task" classes. The AudioIndexingTask class 
/// is used to execute this job.
///
/// To run the program, use the following command
///
/// IndexFingerprints [-u <db_url>] [-m <match_type>] <audio_files_dir>
///
/// where the optional argument <match_type> is one of [MSCALE | XSCALE] and 
/// indicates the match algorithm to be used with the index, <db_url> specifies 
/// the location of the database (for TCDataStore it is the directory hosting 
/// the database files, while for CBDataStore it is the server address or host 
/// name), the mandatory argument <audio_files_dir> indicates the directory 
/// containing the audio files to be indexed.
///


#include <iostream>
#include <vector>
#include <algorithm>

#include "ex_common.h"
#include "IndexFingerprints.h"
#include "CmdLineParser.h"

using namespace Audioneex;

void PrintUsage()
{
    std::cout << "\nUSAGE: IndexFingerprints [-u <db_url>] [-m <match_type>] "
                                    "<audio_files_dir>\n\n";
}


int main(int argc, char** argv)
{
    CmdLineParser cmdLine;
    CmdLineOptions_t opts;

    try
    {
        cmdLine.Parse(argv, argc, opts);

        AudioIndexingTask itask (opts.apath);

        //std::cout << opts.db_url << std::endl;

        std::shared_ptr<KVDataStore>
        dstore ( new DATASTORE_T (opts.db_url) );

        dstore->Open( opts.db_op, true, true );

        std::shared_ptr<Indexer> 
        indexer ( Indexer::Create() );
        indexer->SetDataStore( dstore.get() );
        indexer->SetAudioProvider( &itask );
        indexer->SetMatchType( opts.mtype );

        itask.SetFID( opts.FID_base );
        itask.SetDataStore( dstore );
        itask.SetIndexer( indexer );
        itask.Run();

        std::cout << "Done" << std::endl;
    }
    catch(const bad_cmd_line_exception &ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        PrintUsage();
        return -1;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

