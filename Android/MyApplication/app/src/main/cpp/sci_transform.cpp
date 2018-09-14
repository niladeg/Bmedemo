//
//  sci_transform.cpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 18/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//
#include "sci_transform.hpp"
#include <algorithm>
#include <math.h>
#include <cmath>
#include <numeric>


using namespace std::complex_literals;


namespace sci_modem
{
    namespace sci_transform{
        ComplexVectorD hilbert::transform(std::vector<int16_t > &X)
        {
            ComplexVectorD transformed;
            transformed.reserve(X.size());
            for(auto i=X.begin();i<X.end();++i)
            {
                float Xi = (float)(*i);
                sci_complex tmp=DelayBuffer[0]+1if*(lpfFFilterTaps[0] * Xi + rBuffer[0]);
                transformed.push_back(tmp);
                for (int j = 1; j < HILBERT_FIR_ORDER; ++j)
                    rBuffer[j - 1] = lpfFFilterTaps[j] * Xi + rBuffer[j];
                  for (int j = 1; j < HILBERT_DELAY; ++j)
                    DelayBuffer[j - 1] = DelayBuffer[j];
                    DelayBuffer[HILBERT_DELAY-1] = (float)Xi;
                    rBuffer[HILBERT_FIR_ORDER-1] = lpfFFilterTaps[HILBERT_FIR_ORDER] * Xi;
              }
            return transformed;
        };
        sci_vector_d Filter(std::vector<int16_t> &X,sci_vector_d &coeff_ptr)
        {
          
            size_t NUMBER_LPF_COEF=coeff_ptr.size();
            sci_vector_d dbuffer_ptr(NUMBER_LPF_COEF+1,0);

            sci_vector_d Xout;
            for(auto  i=X.begin();i<X.end();++i)
            {
                float Xi = (float)(*i);
                Xout.push_back(coeff_ptr[0] * Xi + dbuffer_ptr[0]);
                for (size_t j = 1; j < NUMBER_LPF_COEF; ++j)
                    dbuffer_ptr[j - 1] = coeff_ptr[j] * Xi + dbuffer_ptr[j];
                
                dbuffer_ptr[NUMBER_LPF_COEF-1] = coeff_ptr[NUMBER_LPF_COEF] * Xi;
                
            }
            
            return Xout;
        };
        
        ComplexVectorD impairments(ComplexVectorD X, float &cfo)
        {
            int i=0;
            ComplexVectorD out;
            for_each(X.begin(), X.end(),  [&out,&i,&cfo](sci_complex x) { out.push_back(x*std::exp(2*(float)M_PI*cfo*1if*(float)i));i++; });
            return out;
        }
        
        
        
        sci_vector_d kLevinsonDurbin(sci_vector_d &R,uint8_t LPC_ORDER)
        {
            sci_vector_d A(LPC_ORDER+1,0);
            sci_vector_d Ap(LPC_ORDER+1,0);
            sci_vector_d K(LPC_ORDER+1,0);
            
            float E      = -R[1]/R[0];
            
            A[0]   =  1.0f;
            Ap[0]  =  E ;
            float v    = ( 1 - E*E) * R[0];
            
            for (int t = 1 ;t<LPC_ORDER; ++t)
            {
                
                float Leg=0;
                for (int j=1;j<t+1;++j)
                    Leg+=Ap[t-j] * (float)R[j];
                
                E = -((float)R[t+1] + Leg) / v;
                
                for (int j=0;j<t;++j)
                    K[j]=Ap[j];
                
                for (int j=0;j<t;++j)
                    Ap[t-1-j]+=E*K[j];
                
                Ap[t]=E;
                v = v* ( 1 - E*E);
                
            }
            for (int j=0;j<LPC_ORDER;++j)
                A[j+1]=(float)Ap[j];
            return A;
        };
        
        sci_vector_d  kAutoCorr(std::vector<int16_t> &_s, uint32_t _window,uint8_t LPC_ORDER )
        {
        
            sci_vector_d _r(LPC_ORDER+1,0);
            float norm=1.0/(float)_window;
            for (int _iCorrLag = 0; _iCorrLag <= LPC_ORDER; ++_iCorrLag) {
                float _sum = 0.0f;
                for (int _j = 0; _j <_window - _iCorrLag; ++_j)
                {
                    float val=(float)_s[_j];
                    float val2=(float)_s[_j + _iCorrLag];
                    _sum += val * val2;
                }
                _r[_iCorrLag] = _sum*norm;
            }
            return _r;
        };
        
        sci_vector_d kLpc(std::vector<int16_t> &_s,uint32_t _window,uint8_t LPC_ORDER )
        {
           
           sci_vector_d r=kAutoCorr(_s,_window,LPC_ORDER);
           r=kLevinsonDurbin(r,LPC_ORDER);
           std::reverse(r.begin(),r.end());
           return r;
            
        };


        
        
    };
};
