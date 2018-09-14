//
//  bmelib.m
//  bmelib
//
//  Created by Konstantin Gedalin on 28/08/2018.
//
#include <fstream>
#import "bmelib.h"
#include "sci_desc.h"
#include "sci_beep_old.h"
#include "sci_transmitter_helper.hpp"
#include <iostream>

enum eroors{
  SUCCESS,
  MODEERROR,
  FREQUENCYERROR,
  REPETITIONERROR,
  SILENCEERROR,
  GAINERROR
};

struct BeepInfo{
    std::string     Mode;
    float           frequency;
    float           silenceLength;
    float           gain;
    std::string     aac;
    size_t          repetitions;
};
const std::vector<float> WorkingFrequencies={1800,4000,8000,10000,12000,14000,15000,17000,18000,18500,19000,19500};
const std::vector<float> WorkingRepetitions={196,196,196,98,98,98,98,98,98,98,98,98};
const std::vector<std::string> AllowedModes={"PSK","Morse","FDM","FDMA","OFDM","DPSK"};
const std::vector<std::string> errors={"SUCCESS",
    "Wrong MODE",
    "Wrong FREQUENCY",
    "Wrong number of repetitions",
    "Wrong silence length",
    "Wrong gain",
};
const std::vector<float> allowedFrequencies();

void save_to_text(std::string filename,std::vector<float> model_data)
{
    std::ofstream file;

    file.open(filename, std::ofstream::out);
    if (file.fail())
        throw std::ios_base::failure(std::strerror(errno));
    for(auto s:model_data){
        file<<s<<"\n";
    }
    file.close();
    
    
}
int isModeLegal(std::string mode)
{
    
    for (auto t:AllowedModes) {
        if (!t.compare(mode)) {
            return SUCCESS;
        }
    }
    return MODEERROR;
};

int isFrequencyLegal(float frequency)
{
   
    for (auto &t :WorkingFrequencies)
    {
        if (t == frequency)
            return SUCCESS;
    }
    return FREQUENCYERROR;
}




sci_vector_d create_beeps(BeepInfo &jdata)
{
    
    
    sci_vector_d  beep;
    if(!jdata.Mode.compare("PSK") || !jdata.Mode.compare("Morse")||!jdata.Mode.compare("PSK24"))
    {
        BeepGenerator b(jdata.frequency);
        beep=b.generate_beep(jdata.aac,std::string(),jdata.Mode);
    }
    else if(!jdata.Mode.compare("OFDM"))
        beep=sci_modem::sci_helper::CreateTransmitterOFDM(jdata.aac,jdata.frequency);
    else if(!jdata.Mode.compare("DPSK"))
        beep=sci_modem::sci_helper::CreateTransmitterPSK(jdata.aac,jdata.frequency);
    else if(!jdata.Mode.compare("FDM"))
        beep=sci_modem::sci_helper::CreateTransmitterFDM(jdata.aac,jdata.frequency);
    else
        beep=sci_modem::sci_helper::CreateTransmitterFDMA(jdata.aac,jdata.frequency);
    
    
    return beep;
}



sci_vector_d createbeep( BeepInfo jdata) {
    
    sci_vector_d data;
    try{
        
        isFrequencyLegal(jdata.frequency);
        isModeLegal(jdata.Mode);
    }
    catch (std::string e)
    {
        std::cout<<e<<std::endl;
        return data;
    }
    
    sci_vector_d beep=create_beeps(jdata);
    sci_vector_d sil((int)(jdata.silenceLength*SC_SAMPLING),0);
    sil.insert(sil.end(),beep.begin(),beep.end());
    for(size_t t=0;t<jdata.repetitions;t++)
        data.insert(data.end(),sil.begin(),sil.end());
    float gain=jdata.gain;
    std::transform(data.begin(), data.end(), data.begin(),
                   [gain](float val) { return val * gain; });
    
    return data;
}

@implementation bmelib
+(NSData*)beepEncode:   (NSString*)     mode
                    Frequency       :(float)frequency
                    silenceLength   :(float)sil
                    repetitions     :(NSInteger)rep
                    AAC             :(NSString *)AAC
                    gain            :(float)gain
                    error           :(NSString **)err
{
    int ERROR=SUCCESS;
    BeepInfo info;
    info.Mode=std::string([mode UTF8String]);
    info.frequency=frequency;
    info.aac=std::string([AAC UTF8String]);
    
    info.repetitions=(int)rep;
    if(info.repetitions<=0)
    {
        ERROR=REPETITIONERROR;
    }
    info.gain=gain;
    if(info.gain<=0)
    {
        ERROR=GAINERROR;
    }
    info.silenceLength=sil;
    if(info.silenceLength<=0)
    {
        ERROR=SILENCEERROR;
    }
    if(ERROR!=SUCCESS)
    {
        if(err!=nil)
        {
            *err=[NSString stringWithUTF8String:errors[ERROR].c_str()];
        }
        return nil;
    }
    sci_vector_d q=createbeep(info);
    std::vector<short> q1;
    for (auto v : q) {
        short k=(short)(32768.0*v);
        q1.push_back(k);
    }
    NSData *p=[NSData dataWithBytes:q1.data() length:q1.size()*sizeof(short)];
    return p;
};

+(NSMutableArray*)allowedModes
{
    id nsstrings = [NSMutableArray new];
    for (auto str : AllowedModes) {
        id nsstr = [NSString stringWithUTF8String:str.c_str()];
        [nsstrings addObject:nsstr];
    }
    
    return nsstrings;
};
+(NSMutableArray*)allowedFrequencies
{
    id rawdata = [NSMutableArray new];
    for (auto v : WorkingFrequencies) {
        id val = [NSNumber numberWithFloat: v];
        [rawdata addObject:val];
    }
    
    
    return rawdata;
};


@end
