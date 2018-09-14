//
//  sci_transmitter.cpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 25/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//


#include "sci_preamble.hpp"
#include "sci_transform.hpp"
#include "sci_bitcorrect.h"
#include "sci_transmitter.hpp"
#include <fstream>
#include <vector>

namespace sci_modem
{
    namespace sci_transmitter{
        sci_vector_d convert_to_real(ComplexVectorD &x)
        {
            sci_vector_d y;
            for(auto &i:x)y.push_back(real(i));
            return y;
        }
        
        void BasicBeepMaker::create_ofdm_trransmitter(float frequency)
        {
            sci_modem::sci_preamble::setup_premble_ofdm(info,frequency);
            preamble=sci_modem::sci_preamble::create_ofdm_chirp_preamble(info);
            ComplexVectorD long_preamble=sci_modem::sci_preamble::create_long_preamble(info);
            preamble.insert(preamble.end(), long_preamble.begin(),long_preamble.end());
        };
        
        void BasicBeepMaker::create_ofdm_payload(std::vector<uint8_t> &bits)
        {
            payload=sci_modem::sci_preamble::modulate_ofdm_symbol(bits,info);
        };
         sci_vector_d BasicBeepMaker::create_ofdm_message()
        {
            ComplexVectorD msg=preamble;
            msg.insert(msg.end(),payload.begin(),payload.end());
            msg=sci_modem::sci_transform::impairments(msg,info.OFDM.cf);
            sci_vector_d real_message=convert_to_real(msg);
            return real_message;
        }
        
        void BasicBeepMaker::create_psk_trransmitter()
        {
            preamble=sci_modem::sci_preamble::create_psk_chirp_preamble(info);
        };
        void BasicBeepMaker::create_fdm_trransmitter()
        {
            preamble=sci_modem::sci_preamble::create_fdm_chirp_preamble(info);
        };
        void BasicBeepMaker::create_fdm_payload(std::vector<uint8_t> &bits)
        {
            payload=sci_modem::sci_preamble::modulate_fdm_symbol(bits,info);
        };
        void BasicBeepMaker::create_fdma_payload(std::vector<uint8_t> &bits)
        {
            payload=sci_modem::sci_preamble::modulate_fdma_symbol(bits,info);
        };
        void BasicBeepMaker::create_psk_payload(std::vector<uint8_t> &bits)
        {
            payload=sci_modem::sci_preamble::modulate_psk_symbol(bits,info);
        };
        sci_vector_d BasicBeepMaker::create_psk_message()
        {
            ComplexVectorD msg=preamble;
            msg.insert(msg.end(),payload.begin(),payload.end());
            float wc=info.PSK.CarrierFrequency/info.SamplingFrequency;
            msg=sci_modem::sci_transform::impairments(msg,wc);
            sci_vector_d real_message=convert_to_real(msg);
            return real_message;
        };
        sci_vector_d BasicBeepMaker::create_fdm_message()
        {
            ComplexVectorD msg=preamble;
            msg.insert(msg.end(),payload.begin(),payload.end());
            sci_vector_d real_message=convert_to_real(msg);
            return real_message;
        };
        sci_vector_d BasicBeepMaker::create_fdma_message()
        {
            ComplexVectorD msg=preamble;
            msg.insert(msg.end(),payload.begin(),payload.end());
            sci_vector_d real_message=convert_to_real(msg);
            return real_message;
        };
        void BasicBeepMaker::setup_psk(float freq,float shift,int nof)
        {
            sci_preamble::setup_psk(info,freq,shift,nof);
         
        };
        
        
        
    };
    
    
    
};
