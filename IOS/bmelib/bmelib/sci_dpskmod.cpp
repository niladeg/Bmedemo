//
//  sci_dpskmod.cpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 17/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#include "sci_dpskmod.hpp"
#ifndef _MS_WIN_
#include <Accelerate/Accelerate.h>
#else
#include "sci_fft.hpp"
#endif
#include <iomanip>
#include <complex>
#include <cmath>
#include <vector>

namespace sci_modem
{
    namespace sci_common{
        
        

sci_vector_d cumsum(sci_vector_d yPhase)
{
    float tmp=0;
    sci_vector_d out;
    for (auto i=yPhase.begin(); i<yPhase.end(); ++i) {
        
        tmp+=(*i);
        out.push_back(tmp);
    }
    return out;
}


/* modulates DPSK using cumsum taking into account angles*/
 ComplexVectorD dpskmod( const std::vector<uint8_t> x,uint8_t Nphases,float Phase_Rotation)
{
 
    sci_vector_d yPhase;
    for(auto i=x.begin();i<x.end();++i)
        yPhase.push_back( 2*M_PI*(*i)/Nphases+Phase_Rotation);
    sci_vector_d yPhaseCum=cumsum(yPhase);
    ComplexVectorD retval;
    for(auto i=yPhaseCum.begin();i<yPhaseCum.end();++i)
        retval.push_back(cos(*i)+1if*sin(*i));
    return retval;
}

 ComplexVectorD fskmod(const std::vector<uint8_t> &x,float freq_sep,int nSamp,float Fs)
 {
 
 float samptime = 1/Fs;
 size_t nRows = x.size();
 float phIncrSamp[2];
 sci_vector_d phaseIncr;
 for(size_t i=0;i<nSamp;++i){
     float t=-(float)i* 2.0*M_PI * freq_sep/2. * samptime;
  phaseIncr.push_back(t);
 }
 phIncrSamp[0] = phaseIncr[1];    // recall that phaseIncr(1,:) = 0
 phIncrSamp[1] = -phaseIncr[1];    // recall that phaseIncr(1,:) = 0

 sci_vector_d Phase;
 float prevPhase = 0;
 for(int iSym = 0;iSym<nRows;++iSym)
 {
//Get the initial phase for the current symbol
 
 float ph1 = prevPhase;
 
 for(int i=0;i<nSamp;++i)
 {
     
     float t;
     if(!x[iSym])
     t=ph1 + phaseIncr[i];
     else
     t=ph1 - phaseIncr[i];
     
     Phase.push_back(t);
 }
 // If in continuous mode, the starting phase for the next symbol is the
 // ending phase of the current symbol plus the phase increment over one
 // sample.
 prevPhase = Phase[nSamp*(iSym+1)-1] + phIncrSamp[x[iSym]];
 }
     ComplexVectorD y;
     
     for(int iSym = 0;iSym<Phase.size();++iSym)
     y.push_back(exp(1if*Phase[iSym]));
     return y;
 }
 
 

/*this realization of angle takes into accoun further diff in uwrapping procedure*/
sci_vector_d angle(ComplexVectorD p)
{
    sci_vector_d an;
    an.reserve(p.size()+1);
    float *t=(float*)an.data()+1;
    for(auto i=p.begin();i<p.end();++i)
       *t++=atan2(imag(*i),real(*i));
    return an;
}
/*differentiate */
sci_vector_d diff(sci_vector_d p)
{
    sci_vector_d d;
    d.reserve(p.size()-1);
    for(int i=(int)p.size();i>0;i--)
        d[i-1]=p[i]-p[i-1];
    return d;
}
/*mode routine*/
sci_vector_d mod(sci_vector_d p,float  v)
{
    sci_vector_d d(p.size());
    for(int i=0;i<p.size();++i)
        d[i]=p[i]-floor((p[i]+M_PI)/v)*v;
    return d;
}

void unwrap(sci_vector_d p,float cutoff)
{
    //unwrap   Unwraps  vector of phase values.
    // Unwrap phase angles.  Algorithm minimizes the incremental phase variation
    // by constraining it to the range [-pi,pi]
    sci_vector_d dp=diff(p);// Incremental phase variations
    
    sci_vector_d dps=mod(dp,(float)(2*M_PI));
   
    for (int i=0; i<p.size()-1; ++i)// Incremental phase corrections
    {
        if(dps[i]==-M_PI && dp[i]>0 )
            dps[i]=M_PI;
        dps[i]=dps[i]-dp[i];
        if(fabs(dp[i])<cutoff)// Ignore correction when incr. variation is < CUTOFF
            dps[i]=0;
    }
    // Integrate corrections and add to P to produce smoothed phase values
    dp=cumsum(dps);
    for (int i=1; i<p.size(); ++i)// Incremental phase corrections
        p[i] += dp[i-1];
    
    
}


void dpskdemod(std::vector<complex<float>> y,uint8_t Phases,float Phase_Rotation)
{
    
    sci_vector_d wrapped=angle(y);
    unwrap(wrapped,M_PI);
    sci_vector_d zPi=diff(wrapped);
    std::vector<int8_t> zOut;
    int8_t tmp=0;
    for(auto i=zPi.begin();i<zPi.end();++i)
    {
        (*i)-=Phase_Rotation;
        (*i)*=(float)Phases/(2*M_PI);
        (*i)-=0.5;
        tmp=(int8_t)ceil(*i);
        // Convert zPi to linear domain; and make hard decisions by rounding to the
        //nearest integer
        // Note: To be consistent with the blocks, we map 0.5 to 0
        if(tmp<0)// Remap to 0:M-1
        tmp+=Phases;
        zOut.push_back(tmp);
    }
    
}
    

        DSPSplitComplex *sci_fft_prepare(ComplexVectorD bits,int NFFT,size_t st, size_t en)
        {
            DSPSplitComplex *tempSplitComplex=new DSPSplitComplex;
            tempSplitComplex->realp = new float[NFFT];
            tempSplitComplex->imagp = new float[NFFT];
            memset(tempSplitComplex->realp,0,NFFT*sizeof(float));
            memset(tempSplitComplex->imagp,0, NFFT*sizeof(float));
            float *re=tempSplitComplex->realp;
            float *im=tempSplitComplex->imagp;
            for (auto i=bits.begin()+st;i<bits.begin()+NFFT-en; i++)
            {
                
                *re++=real(*i);
                *im++=imag(*i);
            }
            return tempSplitComplex;
        }

    
#ifndef _MS_WIN_
        ComplexVectorD fft(ComplexVectorD x,int NFFT,vDSP_DFT_Direction direction,size_t st, size_t en)
        {
          
            vDSP_DFT_Setup setup;
            setup= vDSP_DFT_zop_CreateSetup(NULL, NFFT, direction);// vDSP_DFT_INVERSE);
            DSPSplitComplex *q=sci_fft_prepare(x,NFFT,st,en);
            std::vector<float > realp(NFFT,0);
            std::vector<float > imagp(NFFT,0);
            float *re=realp.data();
            float *im=imagp.data();
            
            vDSP_DFT_Execute(setup, q->realp,q->imagp, re, im);
            ComplexVectorD bits;
            bits.resize(NFFT);
            for (auto i=bits.begin();i<bits.end(); i++)
            {
                (*i)=*re++ + 1if* *im++;
            }
           
            delete [] q->realp;
            delete [] q->imagp;
            
            vDSP_DFT_DestroySetup(setup);
            return bits;
        };
#else
        ComplexVectorD fft(ComplexVectorD x,int NFFT,vDSP_DFT_Direction direction,size_t st, size_t en)
        {
            
            DSPSplitComplex *q=sci_fft_prepare(x,NFFT,st,en);
            float *re=q->realp;
            float *im=q->imagp;
            if(direction==1)
            SCI_FFT(NFFT, q->realp,q->imagp);
            else{
                
                SCI_FFT(-NFFT, q->realp,q->imagp);
            }
            ComplexVectorD bits;
            bits.resize(NFFT);
            for (auto i=bits.begin();i<bits.end(); i++)
            {
                (*i)=*re++ + 1if* *im++;
            }
            
            delete [] q->realp;
            delete [] q->imagp;
            
            return bits;
        };
        
#endif
        
        

    };
};

