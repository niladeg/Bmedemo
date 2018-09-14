//
//  sci_transform.hpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 18/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once

#include "sci_desc.h"



namespace sci_modem
{
    namespace sci_transform{
    
       class hilbert
       {
        #define HILBERT_FIR_ORDER 8
        #define HILBERT_DELAY     (HILBERT_FIR_ORDER>>1)
           
           sci_vector_d lpfFFilterTaps={-0.000050047994083f,  -0.239316705733721f,   0.000080979355495f,  -0.626102953444243f,                   0.0f,   0.626102953444243f,  -0.000080979355495f, 0.239316705733721f,   0.000050047994083f};
           float   rBuffer[HILBERT_FIR_ORDER+1]={0};
           float   DelayBuffer[HILBERT_DELAY+1]={0};
         private:
           hilbert()=default;
       public:
           ComplexVectorD transform(std::vector<int16_t > &X);
       
       
       };
        
        sci_vector_d Filter(std::vector<int16_t> &X,sci_vector_d &coeff_ptr);
        sci_vector_d kLevinsonDurbin(sci_vector_d &R,uint8_t LPC_ORDER);
        sci_vector_d kAutoCorr(std::vector<int16_t> &_s, uint32_t _window,uint8_t LPC_ORDER );
        sci_vector_d kLpc(std::vector<int16_t> &_s,uint32_t _window,uint8_t LPC_ORDER );
        ComplexVectorD impairments(ComplexVectorD X, float &cfo);
    };


};
