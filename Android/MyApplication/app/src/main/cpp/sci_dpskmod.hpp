//
//  sci_dpskmod.hpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 17/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once
#ifndef _MS_WIN_
#include <Accelerate/Accelerate.h>
#else
#include "sci_fft.hpp"
#endif
#include "sci_desc.h"
namespace sci_modem
{
    namespace sci_common{
        using namespace std::complex_literals;
        using namespace std;
        sci_vector_d cumsum(sci_vector_d yPhase);
        ComplexVectorD dpskmod( const std::vector<uint8_t> x,uint8_t Nphases,float Phase_Rotation);
        sci_vector_d angle(ComplexVectorD p);
        sci_vector_d diff(sci_vector_d p);
        sci_vector_d mod(sci_vector_d p,float  v);
        void unwrap(sci_vector_d p,float cutoff);
        void dpskdemod(ComplexVectorD y,uint8_t Phases,float Phase_Rotation);
        ComplexVectorD fft(ComplexVectorD x,int NFFT,vDSP_DFT_Direction direction,size_t st=0, size_t en=0);
        ComplexVectorD fskmod(const std::vector<uint8_t> &x,float freq_sep,int nSamp,float Fs);

    };
    

    

};



