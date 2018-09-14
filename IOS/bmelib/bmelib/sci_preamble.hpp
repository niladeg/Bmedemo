//
//  sci_preamble.hpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 19/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once
#include <iostream>
#include <vector>
#include "sci_base_def.h"
using namespace std;

namespace sci_modem
{
    namespace sci_preamble{
        using namespace std::complex_literals;
        using namespace std;
        void setup_premble_ofdm(ModemDef &tx,float frequency);
        
        ComplexVectorD moreli( ModemDef &tx,size_t identical_parts );
        std::vector<uint8_t> randi01(int N);
        //preambles
        ComplexVectorD create_ofdm_chirp_preamble(ModemDef &tx);
        ComplexVectorD create_psk_chirp_preamble(ModemDef &tx);
        ComplexVectorD create_long_preamble(ModemDef &tx);
        ComplexVectorD create_fdm_chirp_preamble(ModemDef &tx);
        //modulation
        ComplexVectorD modulate_ofdm_symbol(std::vector<uint8_t> &bits,ModemDef &tx);
        ComplexVectorD modulate_psk_symbol(std::vector<uint8_t> &bits,ModemDef &tx);
        ComplexVectorD modulate_fdm_symbol(std::vector<uint8_t> &bits,ModemDef &tx);
        ComplexVectorD modulate_fdma_symbol(std::vector<uint8_t> &bits,ModemDef &tx);
        
        float MoreliSestimator( ModemDef &tx,ComplexVectorD &invec);
        ComplexVectorD  preambleFDE(ComplexVectorD &RF,ComplexVectorD &RS,ModemDef &tx);
        ComplexVectorD equalize(  ModemDef &tx,ComplexVectorD &recv);
        ComplexVectorD demodulate_modem_data(ComplexVectorD & data,ComplexVectorD &preambleGain,ModemDef &tx);
        void setup_psk(ModemDef &tx,float frequency,float FreqShift,int nof);
        float maxabs( ComplexVectorD x);
        void devide( ComplexVectorD &X,float v);
        ComplexVectorD get_column(std::vector<ComplexVectorD> &x,int column);
        std::vector<uint8_t> get_sub_vector(std::vector<uint8_t> &bits,size_t shift,size_t step);
        std::vector<uint8_t> randi01(int N);
        template <class T> std::vector<std::vector<T>> reshape(std::vector<T> vT,int Rows,int Cols)
        {
          
            int sz=(int)vT.size();
            if (sz!=Rows*Cols)
                throw ("Can't transform");
            
            std::vector<std::vector<T>> out;
            for(int i=0;i<Cols;++i)
            {
                int l=i*Rows;
                std::vector<T> r;
                for(int j=l;j<l+Rows;j++)
                 r.push_back(vT[j]);
                out.push_back(r);
            }
            return out;
        }
        
        
    };
    
    
    
};


