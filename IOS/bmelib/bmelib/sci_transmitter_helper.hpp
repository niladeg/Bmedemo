//
//  sci_transmitter_helper.hpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 16/10/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once

#include "sci_base_def.h"
namespace sci_modem
{
    namespace sci_helper{
        
        sci_vector_d  CreateTransmitterOFDM(std::string msg,float freq);
        sci_vector_d  CreateTransmitterPSK(std::string msg,float freq);
        sci_vector_d  CreateTransmitterFDM(std::string code,float freq);
        sci_vector_d  CreateTransmitterFDMA(std::string code,float freq);
        sci_vector_d getdatafromraw(std::string fn);
        void WriteFile(std::string path,std::vector<float> y);
        sci_vector_d AddBeepsToAudio(sci_vector_d &data,std::vector<sci_vector_d>  beeps,std::vector<int> startTime,std::vector<int> endTime,float &sL,float gain);
        sci_vector_d extract_channel(sci_vector_d data,int chan);
        void addBeep(sci_vector_d b,sci_vector_d &L,int st,int en,int sl,float gain);
    };
};

