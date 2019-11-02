/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <set>
#include <boost/lexical_cast.hpp>

/// Custom exception for malformed cmd lines
class bad_cmd_line_exception : public std::logic_error {
 public: explicit bad_cmd_line_exception(const std::string& msg) :
                  std::logic_error(msg) {}
};


/// This structure defines all the supported options
struct CmdLineOptions_t
{
    std::string                    apath     {};
    std::string                    db_url    {"./data"};
    KVDataStore::eOperation        db_op     {KVDataStore::BUILD};
    uint32_t                       FID_base  {0};
    Audioneex::eMatchType          mtype     {Audioneex::MSCALE_MATCH};
    float                          mms       {1};
    Audioneex::eIdentificationType id_type   {Audioneex::FUZZY_IDENTIFICATION};
    Audioneex::eIdentificationMode id_mode   {Audioneex::EASY_IDENTIFICATION};
    float                          b_thresh  {0.7f};
    bool                           list_dev  {false};
    float                          offset    {0};
};


/// A simple command line parser with no ambitions
class CmdLineParser
{
    std::set<std::string>     m_SupportedOptions;
    std::vector<std::string>  m_Args;

    bool ValidOptions()
    {
        for(auto &arg : m_Args)
        {
            if(arg.substr(0,1) == "-" &&
               m_SupportedOptions.find(arg) == m_SupportedOptions.end())
               return false;
        }
			   
        return true;
    }
	
    template<typename T>
    bool GetOptionValue(const std::string& opt, T& val)
    {
        auto it = std::find(m_Args.begin(), m_Args.end(), opt);
		
        if(it != m_Args.end())
        {
           if(++it != m_Args.end())
              val = boost::lexical_cast<T>(*it);
           else
              throw bad_cmd_line_exception("Invalid command line");
		  
           return true;
        }
		
        return false;
    }
	
    bool OptionExists(const std::string& opt)
    {
        return
        std::find(m_Args.begin(), m_Args.end(), opt) != m_Args.end();
    }

 public:

    CmdLineParser()
	{
        m_SupportedOptions.insert("-f");
        m_SupportedOptions.insert("-m");
        m_SupportedOptions.insert("-i");
        m_SupportedOptions.insert("-d");
        m_SupportedOptions.insert("-b");
        m_SupportedOptions.insert("-l");
        m_SupportedOptions.insert("-u");
        m_SupportedOptions.insert("-s");
    }

    void Parse(char** argv, int argc, CmdLineOptions_t& opts)
    {
         m_Args.clear();
         m_Args.insert(m_Args.end(), argv, argv + argc);

         if(m_Args.size() < 2)
            throw bad_cmd_line_exception("Invalid command line");

         if(!ValidOptions())
            throw bad_cmd_line_exception("Invalid options");

         // Get the mandatory option
         opts.apath = m_Args[m_Args.size()-1];

         // Look for -u argument
         GetOptionValue("-u", opts.db_url);

         // Look for -l argument
         if(OptionExists("-l")){
            opts.list_dev = true;
            return;
         }

         // Look for -f argument
         GetOptionValue("-f", opts.FID_base);

         // Look for -m argument
         std::string mstr;
         if(GetOptionValue("-m", mstr))
         {
            if(mstr != "MSCALE" && mstr != "XSCALE")
               throw std::invalid_argument("Invalid match type");
            opts.mtype = mstr == "MSCALE" ? Audioneex::MSCALE_MATCH :
                                            Audioneex::XSCALE_MATCH;
         }

         // Look for -i argument
         std::string istr;
         if(GetOptionValue("-i", istr))
         {
            if(istr != "BINARY" && istr != "FUZZY")
               throw std::invalid_argument("Invalid identification type");
            opts.id_type = istr == "BINARY" ? Audioneex::BINARY_IDENTIFICATION :
                                              Audioneex::FUZZY_IDENTIFICATION;
         }

         // Look for -d argument
         std::string dstr;
         if(GetOptionValue("-d", dstr))
         {
            if(opts.id_type != Audioneex::FUZZY_IDENTIFICATION)
               throw std::invalid_argument("Option -d only allowed with fuzzy identification");
            if(dstr != "STRICT" && dstr != "EASY")
               throw std::invalid_argument("Invalid identification mode");
            opts.id_mode = dstr == "STRICT" ? Audioneex::STRICT_IDENTIFICATION :
                                              Audioneex::EASY_IDENTIFICATION;
         }

         // Look for -b argument
         float bval;
         if(GetOptionValue("-b", bval))
         {
            if(opts.id_type != Audioneex::BINARY_IDENTIFICATION)
               throw std::invalid_argument("Option -b only allowed with binary identification");
            if(bval<0.5 || bval>1)
               throw std::invalid_argument("Invalid -b threshold. Must be a number in [0.5,1].");
            opts.b_thresh = bval;
         }

         GetOptionValue("-s", opts.offset);
    }
};

#endif // CMDLINEPARSER_H
