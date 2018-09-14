#pragma once
#include "sci_desc.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <assert.h>

#define TWOPI 2*M_PI
#define NZIZOBITS 2
const int TSDEF=300;
const int NOMINAL_NUM_OF_REPETITIONS=98;


class BeepGenerator
{
    float _mfrequency=0;

public:
    BeepGenerator(float mfrequency ){_mfrequency=mfrequency;};
	~BeepGenerator(void){};
    
    std::vector<float> GenaratePskData(std::vector<uint8_t> binFullData);
    std::vector<float> GenarateFskData(std::vector<uint8_t> binFullData);
    std::vector<float> generate_beep(std::string code,std::string appInd,std::string mode);
  
};
//minmax
template<typename T> void minmaxnorm(std::vector<T> y,T &ymin,T &ymax)
{
    auto xmax = std::max_element(std::begin(y), std::end(y));
    auto xmin = std::min_element(std::begin(y), std::end(y));
    assert(xmax!=xmin);
    T norm=T(1)/(*xmax-*xmin);
    for(auto i=y.begin();i<y.end();++i)
    {
        T g=(*i)-*xmin;
        *i = (ymax-ymin)*g*norm + ymin;
    }
    
}
