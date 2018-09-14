//
//  sci_fft.cpp
//  ClBeepMaker
//
//  Created by Konstantin Gedalin on 01/11/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//
#ifdef _MS_WIN_
#include <iostream>
#include <math.h>
#include "sci_fft.hpp"
using namespace std;
/***************************************************************
 function static void make_sintbl(int n, float  *sintbl)
 *
 *   description: Creates sin table according with NFFT (Radix)
 *   pareameters: int n - NFFT
 *                float  *sintbl - table array
 *   return : No
 **************************************************************/

static void make_sintbl(int n, float  *sintbl)
{
    int i, n2, n4, n8;
    float  c, s, dc, ds, t;
    
    n2 = n / 2;  n4 = n / 4;  n8 = n / 8;
    t =(float)sin((float)M_PI /(float)n);
    dc = 2.0f * t * t;  ds = (float)sqrt(dc * (2.0f - dc));
    t =  2.0f * dc;  c = sintbl[n4] = 1.0f;  s = sintbl[0] = 0.0f;
    for (i = 1; i < n8; i++) {
        c -= dc;  dc += t * c;
        s += ds;  ds -= t * s;
        sintbl[i] = s;  sintbl[n4 - i] = c;
    }
    if (n8 != 0) sintbl[n8] =(float)sqrt(0.5f);
    for (i = 0; i < n4; i++)
        sintbl[n2 - i] = sintbl[i];
    for (i = 0; i < n2 + n4; i++)
        sintbl[i + n2] = - sintbl[i];
}


static void make_bitrev(int n, int *bitrev)
{
    int i, j, k, n2;
    
    n2 = n>>1;  i = j = 0;
    for ( ; ; ) {
        bitrev[i] = j;
        if (++i >= n) break;
        k = n2;
        while (k <= j) { j -= k; k>>=1; }
        j += k;
    }
}

int SCI_FFT(int n, float  *x, float  *y)
{
    static int    last_n = 0;     /* previous n */
    static int    *bitrev = NULL; /* bit reversal table */
    static float  *sintbl = NULL; /* trigonometric function table */
    int i, j, k, ik, h, d, k2, n4, inverse;
    float  t, s, c, dx, dy;
    
    /* preparation */
    if (n < 0) {
        n = -n;  inverse = 1; /* inverse transform */
    } else     inverse = 0;
    n4 = n>>2;
    if (n != last_n || n == 0) {
        last_n = n;
        if (sintbl != NULL) delete [] sintbl;
        if (bitrev != NULL) delete [] bitrev;
        if (n == 0) return 0;             /* free the memory */
        sintbl = new float[n + n4];
        bitrev = new int[n * sizeof(int)];
        if (sintbl == NULL || bitrev == NULL) {
            std::cout<<"Memory shortage error\n";
            return 1;
        }
        make_sintbl(n, sintbl);
        make_bitrev(n, bitrev);
    }
    
    /* bit reversal */
    for (i = 0; i < n; i++) {
        j = bitrev[i];
        if (i < j) {
            t = x[i];  x[i] = x[j];  x[j] = t;
            t = y[i];  y[i] = y[j];  y[j] = t;
        }
    }
    
    /* transformation */
    for (k = 1; k < n; k = k2) {
        h = 0;  k2 = k + k;  d = n / k2;
        for (j = 0; j < k; j++) {
            c = sintbl[h + n4];
            if (inverse) s = - sintbl[h];
            else         s =   sintbl[h];
            for (i = j; i < n; i += k2) {
                ik = i + k;
                dx = s * y[ik] + c * x[ik];
                dy = c * y[ik] - s * x[ik];
                x[ik] = x[i] - dx;  x[i] += dx;
                y[ik] = y[i] - dy;  y[i] += dy;
            }
            h += d;
        }
    }
    if (inverse) /* divide by n in case of the inverse transformation */
        for (i = 0; i < n; i++) { x[i] /= n;  y[i] /= n; }
    return 0;  /* finished successfully */
}
#endif
