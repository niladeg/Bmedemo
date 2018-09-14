//
//  sci_transmitter.hpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 25/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once
#include "sci_base_def.h"
namespace sci_modem
{
    namespace sci_transmitter{
    
        class BasicBeepMaker
        {
            ComplexVectorD moreli_preamble;
            ComplexVectorD preamble;
            sci_modem::ModemDef info;
            ComplexVectorD payload;
        public:
            int getNfreqs(){
                return (int)info.OFDM.NFRQ;
            };
            void create_ofdm_trransmitter(float frequency);
            void create_ofdm_payload(std::vector<uint8_t> &bits);
           
            void create_psk_trransmitter();
            void create_fdm_trransmitter();
            void create_psk_payload(std::vector<uint8_t> &bits);
            void create_fdm_payload(std::vector<uint8_t> &bits);
            void create_fdma_payload(std::vector<uint8_t> &bits);
            sci_vector_d create_ofdm_message();
            sci_vector_d create_psk_message();
            sci_vector_d create_fdm_message();
            sci_vector_d create_fdma_message();
            void setup_psk(float freq,float shift,int nof);
            
       // private:
            BasicBeepMaker(){};//=default;
            ~BasicBeepMaker(){};//=default;
            
            
        };
        
    
    
    };
    
    
    
};
