//
//  sci_fft.hpp
//  ClBeepMaker
//
//  Created by Konstantin Gedalin on 01/11/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//
#ifdef _MS_WIN_
#pragma once
typedef enum vDSP_ENUM
{ vDSP_DFT_FORWARD = +1, vDSP_DFT_INVERSE = -1 } vDSP_DFT_Direction;
typedef struct DSPSplitComplext {
    float *  realp;
    float *  imagp;
} DSPSplitComplex;

int SCI_FFT(int n, float  *x, float  *y);
#endif
