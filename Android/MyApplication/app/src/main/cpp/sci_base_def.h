//
//  sci_base_def.h
//  AcousticModem
//
//  Created by Konstantin Gedalin on 30/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once
#include <complex>
#include "sci_desc.h"

const std::string version="2.0.1";
const std::string build="1";

namespace sci_modem{
    

    typedef struct
    {
        
        float  SamplingFrequency=SC_SAMPLING;
        float  FreqShift=300;
        size_t  NFFT=512;
        size_t  NFFT_MORELI=256;
        size_t  NFFT_CHIRP=512;
        uint8_t NPHASES=2;
        uint8_t NbitsPerMessage=64;
        uint8_t NRealbitsPerMessage=56;
        uint8_t NbitsPerHexSymbol=4;//hex code
        std::vector<uint8_t> known={0,1,0,0,0,1,1,1};
        float cf=20/NFFT*SamplingFrequency;
        std::vector<int> bins;
        sci_vector_d lpf={ 0.0036f,-0.0078f,0.0191f,-0.0334f,0.0453f,0.9466f,0.0453f,-0.0334f,0.0191f,-0.0078f,0.0036f};
        float  Phase_Rotation=0;
        float wc;
        float wn;
        ComplexVectorD longPreambleOFDM1;
        ComplexVectorD longPreambleOFDM2;
        int     NumGuardBandCarriers[2]={8,7};
       // union {
            
            struct {
                float CarrierFrequency=19000;
                float Threshold=0.5;
                float NFFT=64;
                float FreqShift=10;
                float cf=0;
            }PSK;
            struct {
                int     NCP=16;
                float  CarrierFrequency=20000;
                float  cf=CarrierFrequency/SC_SAMPLING;
                float  Threshold=0.5;
                uint8_t NFRQ=8;//number of data bits 56 + 8 pilot
                uint8_t FRQSTWP=1;
                uint8_t LeftFrq=9;
                float   endFreq=0;
            }OFDM;
            struct {
                size_t d_vlen;
                size_t d_L;
                size_t d_M;
                sci_vector_d d_weights;
            }moreli;
            
        //};
        
        }ModemDef;
};
