//
//  sci_transmitter_helper.cpp
//  AcousticModem
//
//  Created by Konstantin Gedalin on 16/10/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#include <fstream>
#include "sci_dpskmod.hpp"
#include "sci_preamble.hpp"
#include "sci_rs.h"
#include "sci_bitcorrect.h"
#include "sci_transmitter.hpp"
#include "sci_transmitter_helper.hpp"
using namespace sci_modem;


namespace sci_modem
{
    namespace sci_helper{
        sci_vector_d  CreateTransmitterOFDM(std::string msg,float freq)
        {
            sci_modem::sci_transmitter::BasicBeepMaker bm;
            msg=msg+"1300";
            std::string s=sci_modem::sci_common::create_rs(msg);
            std::vector<uint8_t> binData = sci_modem::sci_common::Hex2Bin(s);
            int N=(int)binData.size();
            int R=N%bm.getNfreqs();
            R=bm.getNfreqs()-R;
            if(R)
            {
                for(int i=0;i<R;++i)
                    binData.insert(binData.begin(),0);
            }
        
            bm.create_ofdm_trransmitter(freq);
            bm.create_ofdm_payload(binData);
            return bm.create_ofdm_message();
            
        };
        
        sci_vector_d  CreateTransmitterPSK(std::string code,float freq)
        {
            sci_modem::sci_transmitter::BasicBeepMaker bm;
            string hexBlockBinData=code+"1300";
            std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
            std::vector<uint8_t> binData = sci_modem::sci_common::Hex2Bin(s);
            bm.setup_psk(freq,300,80);
            bm.create_psk_trransmitter();
            bm.create_psk_payload(binData);
            return bm.create_psk_message();
            
        };
        
        sci_vector_d  CreateTransmitterFDM(std::string code,float freq)
        {
            sci_modem::sci_transmitter::BasicBeepMaker bm;
            string hexBlockBinData=code+"1300";
            std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
            std::vector<uint8_t> binData = sci_modem::sci_common::Hex2Bin(s);
            bm.setup_psk(freq,500,200);
            bm.create_fdm_trransmitter();
            bm.create_fdm_payload(binData);
            return bm.create_fdm_message();
            
        };
        sci_vector_d  CreateTransmitterFDMA(std::string code,float freq)
        {
            sci_modem::sci_transmitter::BasicBeepMaker bm;
            string hexBlockBinData=code+"1300";
            std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
            std::vector<uint8_t> binData = sci_modem::sci_common::Hex2Bin(s);
            bm.setup_psk(freq,1000,98);
            bm.create_fdm_trransmitter();
            bm.create_fdm_payload(binData);
            return bm.create_fdma_message();
            
        };
        sci_vector_d getdatafromraw(std::string fn)
        {
            std::ifstream is;
            sci_vector_d rawfilebuffer;
            is.open(fn, std::ios::binary);
            is.seekg(0, std::ios::end);
            size_t filesize=is.tellg();
            is.seekg(0, std::ios::beg);
            rawfilebuffer.resize(filesize/sizeof(float));
            is.read((char *)rawfilebuffer.data(), filesize);
            return rawfilebuffer;
        };
        
        void WriteFile(std::string path,std::vector<float> y)
        {
         std::ofstream FILE(path, std::ios::out | std::ofstream::binary);
         FILE.write(reinterpret_cast<const char*>(&y[0]), y.size() * sizeof(float));
         FILE.close();
        }
        sci_vector_d extract_channel(sci_vector_d data,int chan)
        {
            sci_vector_d d;
            sci_vector_d q(data);
			for(size_t i=(size_t)chan;i<q.size();i+=2)
				d.push_back(q[i]);
			  return d;
        }
        sci_vector_d merge_channels(sci_vector_d &L_,sci_vector_d &R_)
        {
            sci_vector_d d;
            sci_vector_d L=std::move(L_);
            sci_vector_d R=std::move(R_);
            if(L.size()!=R.size())
                throw ("Incomplete Data");
            d.resize(L.size()+R.size());
            int j=0;
            for(int i=0;i<L.size();++i)
            {
                d[j++]=L[i];
                d[j++]=R[i];
            }
            return d;
        }
        
        void addBeep(sci_vector_d b,sci_vector_d &L_,int st,int en,int sl,float gain)
        {
            sci_vector_d L=std::move(L_);
            
            for(int i=st;i<en;i+=((int)b.size()+sl))
            {
                int k=0;
                for(int j=i;j<i+(int)b.size();++j)
                    L[j]=(1-gain)*L[j]+b[k++]*gain;
            }
            L_=std::move(L);
        }
        
        sci_vector_d AddBeepsToAudio(sci_vector_d &data,std::vector<sci_vector_d>  beeps,std::vector<int> startTime,std::vector<int> endTime,float &sL,float gain)
        {
         
            sci_vector_d f=data;
            sL*=SC_SAMPLING;
            //checking length
            for(auto o=endTime.begin();o<endTime.end();++o)
            {
                if (*o>=data.size())
                {
                    throw string("Incompatible schedule start/end parameters ");
                    return data;
                }
            }
            
            sci_vector_d L=extract_channel(f,0);
            sci_vector_d R=extract_channel(f,1);
        
            int j=0;
            for (auto bp=beeps.begin();bp<beeps.end();++bp)
            {
                addBeep((*bp),L,startTime[j],endTime[j],(int)sL,gain);
                addBeep((*bp),R,startTime[j]+((int)(*bp).size()>>1)+((int)sL>>1),endTime[j],sL,gain);
                j++;
            }
            
            sci_vector_d d=merge_channels(L,R);
            
            return d;
        }
        
        
        
    };
};
