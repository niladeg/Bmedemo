#include "sci_desc.h"
#include "sci_beep_old.h"
#include "sci_transmitter_helper.hpp"
#include <jni.h>
#include <string>
#include <android/log.h>

#include "com_beeple_bmesdk_BmeSdk.h"
/*generateSound(String mode,float frequency,String code,float silenceLn,float gain,int repetitions);*/

struct BeepInfo{
    std::string     Mode;
    float           frequency;
    float           silenceLength;
    float           gain;
    std::string     aac;
    size_t          repetitions;
};
sci_vector_d create_beeps(BeepInfo &jdata)
{

    __android_log_print(ANDROID_LOG_INFO, "sdk", "size_t = %s  %s", jdata.Mode.c_str(),jdata.aac.c_str());
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
    sci_vector_d beep=create_beeps(jdata);
    sci_vector_d sil((int)(jdata.silenceLength*SC_SAMPLING),0);
    __android_log_print(ANDROID_LOG_INFO, "sdk", "size_t = %d  %d", (int)beep.size(),(int)jdata.repetitions);
    sil.insert(sil.end(),beep.begin(),beep.end());
    for(size_t t=0;t<jdata.repetitions;t++)
        data.insert(data.end(),sil.begin(),sil.end());
    float gain=jdata.gain;
    std::transform(data.begin(), data.end(), data.begin(),
                   [gain](float val) { return val * gain; });

    __android_log_print(ANDROID_LOG_INFO, "sdk", "size_t = %d  %d", (int)data.size(),(int)jdata.repetitions);
    return data;
}


JNIEXPORT jshortArray JNICALL Java_com_beeple_bmesdk_BmeSdk_generateSound
        (JNIEnv *_env, jclass cls, jbyteArray mode, jfloat freq, jbyteArray code, jfloat sil, jfloat gain, jint rep)
{
    BeepInfo jdata;

    char *strCode =(char*)_env->GetByteArrayElements(code, 0);
    char *strMode =(char*)_env->GetByteArrayElements(mode, 0);
    jdata.Mode=std::string(strMode);

    jdata.aac=std::string(strCode);
    jdata.aac=jdata.aac.substr(0,4);
    jdata.frequency=freq;
    jdata.gain=gain;
    jdata.repetitions=rep;
    jdata.silenceLength=sil;
    sci_vector_d q=createbeep(jdata);
    std::vector<short> q1;
    for (auto v : q) {
        short k=(short)(32768.0*v);
        q1.push_back(k);
    }
    _env->ReleaseByteArrayElements(code,(signed char*)strCode, 0);
    _env->ReleaseByteArrayElements(mode, (signed char*)strMode, 0);
    jshortArray result;

    result = _env->NewShortArray(q1.size());
    _env->SetShortArrayRegion(result, 0, q1.size(), q1.data());
    return result;



}