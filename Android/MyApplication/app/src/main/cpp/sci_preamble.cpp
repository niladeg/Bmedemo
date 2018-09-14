//
//  sci_preamble.cpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 19/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//


#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <complex>
#include <cmath>
#include <iostream>
#include <vector>
#include <valarray>
#include "sci_dpskmod.hpp"
#include "sci_preamble.hpp"

using namespace std::complex_literals;
using namespace std;
using namespace sci_modem::sci_common;
namespace sci_modem
{
    namespace sci_preamble
    {
       
        std::vector<float> LongPreamble={
            0,    0,    0,    0,    0,    0,    0,    0,
            1,    -1,    -1,    1,    -1,    1,    -1,    1,
            1,    1,    1,    -1,    -1,    1,    -1,    1,
            -1,    -1,    -1,    -1,    -1,    1,    -1,    -1,
            1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
            1,    1,    -1,    1,    -1,    1,    -1,    -1,
            -1,    -1,    -1,    1,    -1,    -1,    1,    1,
            1,    -1,    -1,    -1,    1,    -1,    1,    1,
            -1,    1,    -1,    -1,    1,    1,    1,    1,
            -1,    1,    1,    1,    1,    -1,    -1,    1,
            -1,    -1,    -1,    -1,    1,    -1,    1,    -1,
            -1,    -1,    1,    1,    1,    1,    -1,    -1,
            1,    -1,    -1,    1,    1,    -1,    -1,    1,
            1,    1,    1,    -1,    1,    1,    1,    -1,
            -1,    1,    -1,    -1,    1,    1,    -1,    1,
            1,    -1,    1,    -1,    -1,    1,    1,    1,
            1,    -1,    1,    1,    1,    1,    -1,    1,
            1,    1,    1,    -1,    -1,    -1,    1,    -1,
            1,    -1,    -1,    -1,    1,    -1,    1,    1,
            -1,    1,    1,    1,    -1,    -1,    1,    -1,
            1,    1,    -1,    1,    1,    -1,    -1,    -1,
            1,    1,    -1,    1,    1,    1,    -1,    -1,
            1,    1,    -1,    1,    -1,    -1,    1,    1,
            -1,    -1,    -1,    1,    -1,    -1,    -1,    1,
            1,    -1,    -1,    -1,    1,    -1,    -1,    1,
            1,    -1,    1,    -1,    -1,    1,    1,    1,
            1,    1,    -1,    -1,    -1,    1,    1,    -1,
            -1,    -1,    -1,    -1,    -1,    1,    1,    1,
            -1,    -1,    -1,    1,    -1,    -1,    1,    -1,
            1,    1,    1,    -1,    -1,    -1,    -1,    1,
            -1,    1,    1,    -1,    1,    -1,    -1,    -1,
            1,    0,    0,    0,    0,    0,    0,    0};

        std::vector<uint8_t> pn1={1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,
            1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,
            1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,
            1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,
            0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,
            0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,
            1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,
            0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,
            1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,
            1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,
            1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,
            0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,
            0,0,};
        std::vector<uint8_t> pn2={1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,
            1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,
            0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,
            1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,
            1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,
            1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,
            0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,
            0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,
            1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,
            0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,
            1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,
            1,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0,
            1,1,1,};
        std::vector<uint8_t> pn4={1,1,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,
            1,1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,1,0,1,
            0,0,0,1,1,0,0,0,1,1,1,0,1,1,0,0,0,1,0,1,
            0,1,0,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0,0,1,
            0,0,1,1,1,1,0,1,1,0,1,0,0,1,1,1,0,1,0,0,
            0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,1,1,0,
            0,0,1,0,1,1,1,0,0,0,1,0,1,0,0,0,0,1,1,1,
            0,1,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,
            0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,
            0,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,1,
            1,1,0,1,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,
            0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,
            1,0,0,1,0,1,1,0,1,0,0,1,1,1,0,0,1};
        std::vector<uint8_t> pn5={1,0,1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,
            1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,
            0,0,0,0,1,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,
            0,0,1,1,1};
        std::vector<uint8_t> PskKnownBits={1,0,0};
        std::vector<uint8_t> FskKnownBits={1,1,0,0};

        std::vector<float> ham(int n) {
            float k = (float) (2 * M_PI) / ((float) (n - 1)); /* 2*pi/(N-1) */
            std::vector<float> w;
            for (int i = 0; i < n; i++)
                w.push_back((float) 0.54 - (float) 0.46 * (float) cos(k * (float) i));
            return w;
        }
        ComplexVectorD create_psk_chirp_preamble(ModemDef &tx)
        {
            ComplexVectorD ModulatedWaveform(tx.NFFT_CHIRP,0);
            ComplexVectorD preamble(15,0);//this is hard coded to avoid stupid questions

            //ModulatedWaveform.reserve(tx.NFFT_CHIRP);
            float wc=tx.PSK.CarrierFrequency/tx.SamplingFrequency;
            float wn=(tx.PSK.CarrierFrequency+tx.PSK.FreqShift)/tx.SamplingFrequency;
            tx.wc=wc;
            tx.wn=wn;
            std::vector<float> h=ham((int)tx.NFFT_CHIRP);
            for(size_t samples=0;samples<tx.NFFT_CHIRP;++samples)
                      ModulatedWaveform[samples]=exp( (float)M_PI*1if*(wn-wc)/((float)tx.NFFT_CHIRP)*(float)samples*(float)samples)*h[samples];
            preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());//concatenate vectors
            preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());
            //preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());//concatenate vectors
            //preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());
            float v=maxabs( preamble);
            devide(preamble,v);
            return preamble;
        
        };
        ComplexVectorD create_fdm_chirp_preamble(ModemDef &tx)
        {
            ComplexVectorD ModulatedWaveform(tx.NFFT_CHIRP,0);
            ComplexVectorD preamble(15,0);//this is hard coded to avoid stupid questions
            
            float wc=(tx.PSK.CarrierFrequency-tx.PSK.FreqShift/2)/tx.SamplingFrequency;
            float wn=(tx.PSK.CarrierFrequency+tx.PSK.FreqShift/2)/tx.SamplingFrequency;
            tx.wc=wc;
            tx.wn=wn;
            std::vector<float> h=ham((int)tx.NFFT_CHIRP);
            for(size_t samples=0;samples<tx.NFFT_CHIRP;++samples)
           
              ModulatedWaveform[samples]=exp( 2*(float)M_PI*1if*(wc*(float)samples+(wn-wc)/(2*(float)tx.NFFT_CHIRP)*(float)samples*(float)samples))*h[samples];
            preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());//concatenate vectors
            preamble.insert(preamble.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());
          
            float v=maxabs( preamble);
            devide(preamble,v);
            return preamble;
            
        };
        ComplexVectorD create_ofdm_chirp_preamble(ModemDef &tx)
        {
            ComplexVectorD ModulatedWaveform;
            ComplexVectorD preamble(tx.OFDM.NCP,{0});
            float wc=tx.OFDM.CarrierFrequency/tx.SamplingFrequency;
            float wn=(tx.OFDM.CarrierFrequency+tx.FreqShift)/tx.SamplingFrequency;
            tx.wc=wc;
            tx.wn=wn;
            std::vector<float> h=ham((int)tx.NFFT_CHIRP/2);
            for(size_t samples=0;samples<tx.NFFT_CHIRP/2;++samples)
            {
                ModulatedWaveform.push_back(exp( 2*(float)M_PI*1if*(/*wc*(float)samples+*/(wn-wc)/(2*(float)tx.NFFT_CHIRP)*(float)samples*(float)samples)));
                
            }
            ComplexVectorD A;
            A.insert(A.end(),preamble.begin(),preamble.end());//concatenate vectors
            A.insert(A.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());
            A.insert(A.end(),ModulatedWaveform.begin(),ModulatedWaveform.end());
            float v=maxabs( A);
            devide(A,v);
            
            return A;
            
        };

        
        std::vector<uint8_t> randi01(int N)
        {
            
            std::vector<uint8_t> out;
            for(int i=0;i<N;++i)
            {
                int x=rand()%2;
                out.push_back((uint8_t)x);
                
            }
            return out;
        };
        

        ComplexVectorD moreli( ModemDef &tx,size_t identical_parts )
        {
        
         tx.moreli.d_vlen=tx.NFFT_MORELI;
         tx.moreli.d_L=identical_parts;
         tx.moreli.d_M=tx.moreli.d_vlen/identical_parts;
         size_t L = identical_parts;
         size_t H = size_t(L/2);
        
            for(size_t m =1;m<=H;++m)
            {
                    
                    size_t mf = m;
                    float w = 3 * ((L-mf)*(L-mf+1)-H*(L-H))/(H*(4*(H*H)-6*L*H+3*(L*L)-1));
                    tx.moreli.d_weights.push_back(w);
            }
            
            std::vector<uint8_t> x=pn4;//randi01((int)tx.NFFT_MORELI);
            ComplexVectorD z=dpskmod(x,tx.NPHASES, tx.Phase_Rotation);
            ComplexVectorD y=fft(z,(int)tx.NFFT_MORELI,vDSP_DFT_INVERSE);
            ComplexVectorD preamble(15,0);
            size_t t=y.size()/4;
            preamble.insert(preamble.end(),y.begin(),y.begin()+t);//concatenate vectors
            preamble.insert(preamble.end(),y.begin(),y.begin()+t);//concatenate vectors
            preamble.insert(preamble.end(),y.begin(),y.begin()+t);
            preamble.insert(preamble.end(),y.begin(),y.begin()+t);//concatenate vectors
            float v=maxabs( preamble);
            devide(preamble,v);
           
            return preamble;
        };
        float MoreliSestimator( ModemDef &tx,ComplexVectorD &invec)
        {
        
         size_t H =tx.moreli.d_L/2;
         ComplexVectorD R(H+1,1.0f+1if*0.0f);
        
            for(size_t m = 0;m<H;++m)
            {
             complex<float> acc  =complex<float>(0.0f+1if*0.0f);
             size_t shift = m*tx.moreli.d_M;
            
                for (size_t k = shift;k<tx.moreli.d_vlen;++k)
                 acc =acc + invec[k]*conj(invec[k-shift]);
                
                R[m] = acc;
            }
                
                // estimate the frequency offset
            float est = 0.0;
            for (size_t m = 1;m<=H;++m)
            {
                    complex<float> t = R[m] *conj(R[m-1]);
                    float phase_diff = atan2(imag(t),real(t));
                    est = est+tx.moreli.d_weights[m-1] * phase_diff;
            }
                    
                    est = tx.moreli.d_L / (2*M_PI) * est;
            return est;
        };

        ComplexVectorD create_long_preamble(ModemDef &tx)
        {
        
            int n=(int)(tx.NFFT/2);
            ComplexVectorD bits(n,0);
          
            for(int i=0;i<(int)tx.bins.size();++i)
            {
                if(pn4[tx.bins[i]]==0)
                    bits[tx.bins[i]].real(-1);
                else
                    bits[tx.bins[i]].real(1);
            }
            
         ComplexVectorD y_h=fft(bits,(int)tx.NFFT/2,vDSP_DFT_INVERSE);
            tx.longPreambleOFDM1=y_h;
            tx.longPreambleOFDM2=y_h;
            
         ComplexVectorD y;
           
            for(auto i=y_h.end()-1;i>=y_h.end()-tx.OFDM.NCP;--i)
            {
             y.push_back(*i);
            }
           y.insert(y.end(),y_h.begin(),y_h.end());//concatenate vectors
           y.insert(y.end(),y_h.begin(),y_h.end());
           float v=maxabs( y);
           devide(y,v);
           return y;
            
        };
        
       ComplexVectorD  preambleFDE(ComplexVectorD &RF,ComplexVectorD &RS,ModemDef &tx)
        {
        // Calculate non-normalized channel gains
            std::vector<std::complex<float > > RGains;
            int j=tx.NumGuardBandCarriers[0];
            for (size_t t=tx.NumGuardBandCarriers[0];t<tx.NFFT-tx.NumGuardBandCarriers[1];++t)
            {
                std::complex<float > RNormal_F = RF[t]/tx.longPreambleOFDM1[t-j]; // Known is the original Long Preamble symbols
                std::complex<float > RNormal_S = RS[t]/tx.longPreambleOFDM2[t-j];
                // Scale channel gains
                float REnergy = (norm(RNormal_F)+norm(RNormal_S));
                std::complex<float > RConj = conj(RNormal_F+RNormal_S);
                RGains.push_back(RConj/ REnergy);
            }
        return RGains;
        };
        ComplexVectorD equalize(  ModemDef &tx,ComplexVectorD &recv)
        {
        // equalizeOFDM: Equalize the entire OFDM frame through the use of both
        // the long preamble from the 802.11a standard and four pilot tones in
        // the data frames themselves.  The gains from the pilots are
        // interpolated across frequency to fill the data frame and apply gains
        // to all data subcarriers.
        //// Use Long Preamble frame to estimate channel in frequency domain
        // Separate out received preambles
        vector<std::complex<float >> rLongFirst  = fft(recv,(int)tx.NFFT,vDSP_DFT_FORWARD,tx.OFDM.NCP);
        vector<std::complex<float >> rLongSecond = fft(recv,(int)tx.NFFT/2,vDSP_DFT_FORWARD,tx.OFDM.NCP+tx.NFFT/2);
        return preambleFDE(rLongFirst ,rLongSecond ,tx);
        
    
        };
        
        // Calculate Equalizer Taps with preamble symbols
        
        
         ComplexVectorD demodulate_modem_data(ComplexVectorD & data,ComplexVectorD &preambleGain,ModemDef &tx)
        {
            
            float coef=(float)1/(tx.NFFT/2);
            ComplexVectorD fftOut=fft(data,(int)tx.NFFT/2,vDSP_DFT_FORWARD,tx.OFDM.NCP);
            ComplexVectorD equalized_array;
            for(auto i=tx.bins.begin();i<tx.bins.end();++i)
                equalized_array.push_back(coef*fftOut[*i]*preambleGain[*i+tx.NumGuardBandCarriers[0]]);
            return equalized_array;
        };

        std::vector<uint8_t> get_sub_vector(std::vector<uint8_t> &bits,size_t shift,size_t step)
        {
            std::vector<uint8_t> y;
            for(auto i=bits.begin()+shift;i<bits.end();i+=step)
            y.push_back(*i);
            return y;
        }
        ComplexVectorD get_column(std::vector<ComplexVectorD> &x,int column)
        {
            ComplexVectorD y;
            for (auto  i=x.begin();i<x.end();++i)
            {
               // ComplexVectorD a=*i;
                y.push_back((*i)[column]);
            }
            return y;
            
        }
        float maxabs( ComplexVectorD x)
        {
            float maxval=abs(x[0]);
            for(auto i=x.begin()+1;i<x.end();++i)
            {
                if(maxval<abs(*i))
                    maxval=abs(*i);
            }
            return maxval;
        }
        void devide( ComplexVectorD &X_,float v)
        {
            ComplexVectorD X=std::move(X_);
            for_each(X.begin(), X.end(),  [&v](sci_complex &x) {
                x/=v;
            
            });
            X_=std::move(X);
        }
#if 0
        ComplexVectorD modulate_ofdm_symbol(std::vector<uint8_t> &bits_,ModemDef &tx)
        {
            //single symbol include 8 known bits and as pilot bits if is needed
            //
            std::vector<uint8_t> bits=std::move(bits_);
            std::vector<ComplexVectorD> message;
          
            size_t NumberOfShortMessages=bits.size()/tx.OFDM.NFRQ;
            for(size_t i=0;i<bits.size();i+=NumberOfShortMessages)
            {
             std::vector<uint8_t> shortbits=get_sub_vector(bits,i,NumberOfShortMessages);
             ComplexVectorD z=dpskmod(shortbits,tx.NPHASES, tx.Phase_Rotation);
             message.push_back(z);
            }
            ComplexVectorD to_send;
            to_send.reserve(NumberOfShortMessages*(tx.NFFT+tx.OFDM.NCP));
            for(int j=0;j<NumberOfShortMessages;j++)
            {
                ComplexVectorD z=get_column(message,j);
                ComplexVectorD d(tx.NFFT,0);
                for(int i=0;i<(int)tx.bins.size();++i)
                d[tx.bins[i]]=z[i];
            
                ComplexVectorD y=fft(d,(int)tx.NFFT,vDSP_DFT_INVERSE);
                float v=maxabs( y);
                devide(y,v);
                for(size_t i=y.size()-1;i>=y.size()-tx.OFDM.NCP;--i )
                to_send.push_back(y[i]);
                to_send.insert(to_send.end(),y.begin(),y.end());
               
                
            }
            return to_send;
        };

        
        
        
        ComplexVectorD modulate_ofdm_symbol(std::vector<uint8_t> &bits_,ModemDef &tx)
        {
            //single symbol include 8 known bits and as pilot bits if is needed
            //
            std::vector<uint8_t> bits=std::move(bits_);
            std::vector<ComplexVectorD> message;
            std::vector<uint8_t> x=pn5;//=randi01((int)tx.OFDM.NFRQ);
            size_t NumberOfShortMessages=bits.size()/tx.OFDM.NFRQ;
            std::vector<std::vector<uint8_t>> u=reshape(bits,(int)tx.OFDM.NFRQ,(int)NumberOfShortMessages);
            for(int i=0;i<(int)tx.OFDM.NFRQ;i++)
            {
                std::vector<uint8_t> shortbits;
                shortbits.push_back(x[i]);
                for(int j=0;j<NumberOfShortMessages;++j)
                shortbits.push_back(u[j][i]);
                ComplexVectorD z=dpskmod(shortbits,tx.NPHASES, tx.Phase_Rotation);
                message.push_back(z);
               
            
            }
           
            
            ComplexVectorD to_send;
            to_send.reserve((NumberOfShortMessages+1)*(tx.NFFT/2+tx.OFDM.NCP));
            for(int j=0;j<NumberOfShortMessages+1;j++)
            {
                ComplexVectorD z=get_column(message,j);
                ComplexVectorD d(tx.NFFT/2,0);
                for(int i=0;i<(int)tx.bins.size();++i)
                    d[tx.bins[i]]=z[i];
                
                ComplexVectorD y=fft(d,(int)tx.NFFT/2,vDSP_DFT_INVERSE);
                float v=maxabs( y);
                devide(y,v);
                for(size_t i=y.size()-tx.OFDM.NCP;i<=y.size()-1;++i )
                    to_send.push_back(y[i]);
                to_send.insert(to_send.end(),y.begin(),y.end());
                
                
            }
           
            return to_send;
        };
 #endif
        ComplexVectorD modulate_ofdm_symbol(std::vector<uint8_t> &bits_,ModemDef &tx)
        {
            std::vector<uint8_t> bits=std::move(bits_);
            ComplexVectorD message;
            std::vector<uint8_t> x={0,1,1,1,1,1,1,1};//randi01((int)tx.OFDM.NFRQ);
            x.insert(x.end(),bits.begin(),bits.end());
            size_t NumberOfShortMessages=x.size()/tx.OFDM.NFRQ;
            
            for(size_t i=0;i<tx.OFDM.NFRQ;i++)
            {
                std::vector<uint8_t> shortbits=get_sub_vector(x,i,tx.OFDM.NFRQ);
                ComplexVectorD z=dpskmod(shortbits,tx.NPHASES, tx.Phase_Rotation);
                message.insert(message.end(),z.begin(),z.end());
            }
            
            ComplexVectorD to_send;
            to_send.reserve((NumberOfShortMessages)*(tx.NFFT/2+tx.OFDM.NCP));
            for(int j=0;j<NumberOfShortMessages;j++)
            {
                ComplexVectorD z;
                for(auto i=message.begin()+j;i<message.end();i+=NumberOfShortMessages)
                    z.push_back(*i);
                
                ComplexVectorD d(tx.NFFT/2,0);
                for(int i=0;i<(int)tx.OFDM.NFRQ;++i)
                    d[tx.bins[i]]=z[i];
                
                ComplexVectorD y=fft(d,(int)tx.NFFT/2,vDSP_DFT_INVERSE);
                float v=maxabs( y);
                devide(y,v);
                for(size_t i=y.size()-tx.OFDM.NCP;i<=y.size()-1;++i )
                    to_send.push_back(y[i]);
                to_send.insert(to_send.end(),y.begin(),y.end());
                
                
            }
            
            return to_send;
        };
        
        ComplexVectorD modulate_psk_symbol(std::vector<uint8_t> &bits,ModemDef &tx)
        {
            
            std::vector<uint8_t> bits_=std::move(bits);
            std::vector<uint8_t> tmp(PskKnownBits);
            tmp.insert(tmp.end(),bits_.begin(),bits_.end());
            ComplexVectorD z=dpskmod(tmp,tx.NPHASES, tx.Phase_Rotation);
            ComplexVectorD y;
            std::vector<float> h=ham((int)tx.NFFT_CHIRP);
            for(auto i=z.begin();i<z.end();++i)
            {
                for(int j=0;j<tx.PSK.NFFT;++j)
                    y.push_back((*i)*h[j]);
                
            }
            
            
          // bits=std::move(bits_);
            return y;
        };
        
        ComplexVectorD modulate_fdm_symbol(std::vector<uint8_t> &bits,ModemDef &tx)
        {
            
            std::vector<uint8_t> bits_=std::move(bits);
            std::vector<uint8_t> tmp(FskKnownBits);
            tmp.insert(tmp.end(),bits_.begin(),bits_.end());
            std::vector<float> h=ham((int)tx.NFFT_CHIRP);
            ComplexVectorD  o(tx.NFFT_CHIRP);
            ComplexVectorD  z(tx.NFFT_CHIRP);
            for (int t=0; t<tx.NFFT_CHIRP; ++t) {
                o[t]=h[t]*exp(2*(float)M_PI*1if*tx.wc*(float)t);
                z[t]=h[t]*exp(2*(float)M_PI*1if*tx.wn*(float)t);
            }
            
            ComplexVectorD y;
             
            for(auto i=tmp.begin();i<tmp.end();++i)
            {
                    if(*i)
                    y.insert(y.end(),o.begin(),o.end());
                    else
                    y.insert(y.end(),z.begin(),z.end());

            }
    
            return y;
            
        };
        ComplexVectorD modulate_fdma_symbol(std::vector<uint8_t> &bits,ModemDef &tx)
        {
            
            std::vector<uint8_t> bits_=std::move(bits);
            std::vector<uint8_t> tmp(FskKnownBits);
            tmp.insert(tmp.end(),bits_.begin(),bits_.end());
        
             ComplexVectorD z=fskmod(tmp,tx.PSK.FreqShift,(int)tx.NFFT_CHIRP,tx.SamplingFrequency);
             ComplexVectorD y;
             std::vector<float> h=ham((int)tx.NFFT_CHIRP);
            
             float fc=2*M_PI*tx.PSK.CarrierFrequency/tx.SamplingFrequency;
             for(int i=0;i<(int)z.size();i+=tx.NFFT_CHIRP)
             {
                 for(size_t j=0;i<tx.NFFT_CHIRP;++j)
                 {
                     complex<float> t=z[i+j]*exp(1if*(float)j*fc);
                     y.push_back(t*h[j]);
                 }
             }
            return y;
            
        };
        
        
        
        void setup_premble_ofdm(ModemDef &tx,float frequency)
        {
        
            uint8_t j=tx.OFDM.FRQSTWP;
            uint8_t l=tx.OFDM.LeftFrq;
            for(size_t i=l;i<l+j*tx.OFDM.NFRQ;i+=j)
                   tx.bins.push_back((int)i);
            float BandFrq=(tx.bins[0]+1)*tx.SamplingFrequency/tx.NFFT*2;
            tx.OFDM.CarrierFrequency=frequency;
            tx.OFDM.cf=(tx.OFDM.CarrierFrequency-BandFrq)/tx.SamplingFrequency;
            size_t i=tx.bins.size();
            tx.OFDM.endFreq=tx.bins[i-1]*tx.SamplingFrequency/tx.NFFT*2;
        };
        
        void setup_psk(ModemDef &tx,float frequency,float FreqShift,int nof)
        {
            tx.PSK.CarrierFrequency=frequency;
            tx.PSK.FreqShift=FreqShift;
            tx.PSK.NFFT=nof;
            tx.NFFT_CHIRP=nof;
            tx.PSK.cf=frequency/tx.SamplingFrequency;
            
        };
        
        
        
    };
    
    
    
};

