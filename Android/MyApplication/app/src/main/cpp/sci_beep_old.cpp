#include "sci_bitcorrect.h"
#include "sci_beep_old.h"
#include <math.h>
#include <android/log.h>

using namespace sci_modem::sci_common;


std::vector<float> hamming(int n) {
    float k = (float) (2 * M_PI) / ((float) (n - 1)); /* 2*pi/(N-1) */
    std::vector<float> w;
     for (int i = 0; i < n; i++)
        w.push_back((float) 0.54 - (float) 0.46 * (float) cos(k * (float) i));
    return w;
}

std::vector<float> BeepGenerator::GenarateFskData(std::vector<uint8_t>  binFullData)
{
    int DataLength          =   (int)binFullData.size();
    int   Ts                =  TSDEF;
    float CenterFreq        =   _mfrequency;
    float SecondFrequency   =   CenterFreq;
    float FirstFrequency    =   CenterFreq-(float)SC_SAMPLING/(float)Ts*(float)10;
    std::vector<float> a    =   hamming(Ts);
    float wc2 =(float)(TWOPI * SecondFrequency/SC_SAMPLING);
    float wc1 =(float)(TWOPI * FirstFrequency/SC_SAMPLING);
    vector<float>  o(Ts);
    vector<float>  z(Ts);
    for (int t=0; t<Ts; ++t) {
        o[t]=a[t]*cos(wc1*t);
        z[t]=a[t]*cos(wc2*t);
    }
    
    std::vector<float> ModulatedWaveform;
    ModulatedWaveform.insert(ModulatedWaveform.end(),o.begin(),o.end());
    ModulatedWaveform.insert(ModulatedWaveform.end(),o.begin(),o.end());
    ModulatedWaveform.insert(ModulatedWaveform.end(),z.begin(),z.end());
    ModulatedWaveform.insert(ModulatedWaveform.end(),z.begin(),z.end());
    for(int iBit=0;iBit<DataLength;++iBit)
    {
        if (binFullData[iBit])
             ModulatedWaveform.insert(ModulatedWaveform.end(),o.begin(),o.end());
        else ModulatedWaveform.insert(ModulatedWaveform.end(),z.begin(),z.end());
    }
    

    float ymin=-1,ymax=1;
    minmaxnorm(ModulatedWaveform,ymin,ymax);
    return ModulatedWaveform;
}
std::vector<float> BeepGenerator::GenaratePskData(std::vector<uint8_t> binFullData)
{
    int  DataLength=(int)binFullData.size();
    int Ts                      =    NOMINAL_NUM_OF_REPETITIONS;
    std::vector<float> a        =    hamming(Ts);
    float wc =(float)(TWOPI * _mfrequency/SC_SAMPLING);
    int NumberSamplesPerRecord=Ts * (DataLength + 2*NZIZOBITS);
    std::vector<float> ModulatedWaveform (NumberSamplesPerRecord,0);
    
    int ModulatedIndex = 0;
    float CurrPhase =(float) M_PI;
    float LastPhase = 0;
    
    for (int i1 = 0; i1<NZIZOBITS;++i1)
    {
        for (int i2 = 0;i2<Ts;++i2)
        {
            ModulatedWaveform[ModulatedIndex] = a[i2]*sin((float)ModulatedIndex * wc + CurrPhase);
            ModulatedIndex++;
        }
        LastPhase = CurrPhase;
        CurrPhase +=(float)M_PI;
        
    } // end of preamble generation
    LastPhase = atan2(sin(LastPhase),cos(LastPhase));
    LastPhase=(LastPhase < 0)?LastPhase += float(TWOPI):LastPhase;
    CurrPhase=LastPhase;
    for (int i1 = 0; i1<NZIZOBITS;++i1)
    {
        for (int i2 = 0;i2<Ts;++i2)
        {
            ModulatedWaveform[ModulatedIndex] = a[i2]*sin((float)ModulatedIndex * wc + CurrPhase);
            ModulatedIndex++;
        }
        LastPhase = CurrPhase;
    } // end of preamble generation
    LastPhase = atan2(sin(LastPhase),cos(LastPhase));
    LastPhase=(LastPhase < 0)?LastPhase += float(TWOPI):LastPhase;
    
   
    for ( int i1=0; i1< DataLength; i1++)
    {
        float   CurrentData = (binFullData[i1]<<1);
        CurrPhase = LastPhase + CurrentData * (float)(M_PI);
        CurrPhase = atan2(sin(CurrPhase),cos(CurrPhase));
        if (CurrPhase < 0)
			CurrPhase +=float(TWOPI);
        
        for(int i2 = 0;i2<Ts;++i2){
            ModulatedWaveform[ModulatedIndex] = a[i2]*sin( (float)(ModulatedIndex * wc) + CurrPhase);
            ModulatedIndex++;
        }
        LastPhase = CurrPhase;
    }
    
    return ModulatedWaveform;

}



std::vector<float> BeepGenerator::generate_beep(std::string code,string appInd, string mode)
{
    std::vector<uint8_t> binData;
    unsigned char *p;
        if(!mode.compare("PSK24"))
        {
            std::string hexBlockBinData=code+"0000";
            std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
            binData = sci_modem::sci_common::Hex2Bin(s);
        }
        else
        {
            if(appInd.size()!=0)
            {
             std::string hexBlockBinData=appInd+"1301";
             std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
             binData = sci_modem::sci_common::Hex2Bin(s);
             s=sci_modem::sci_common::create_rs(code);
             std::vector<uint8_t> binDataBlock1 = sci_modem::sci_common::Hex2Bin(s);
             binData.insert(binData.end(),binDataBlock1.begin(),binDataBlock1.end());
            }else{
                std::string hexBlockBinData=code+"1300";
                std::string s=sci_modem::sci_common::create_rs(hexBlockBinData);
                binData = sci_modem::sci_common::Hex2Bin(s);
                p=binData.data();
               
            }
        }
    __android_log_print(ANDROID_LOG_INFO, "sdk", "size_t = %d", (int)binData.size());
        vector<float> t;
        if (!mode.compare("PSK") || !mode.compare("PSK24"))
            t= GenaratePskData(binData);
        else
            t=GenarateFskData(binData);
   
    return t;
   
}
