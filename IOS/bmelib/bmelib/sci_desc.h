//
//  sci_desc.h
//  AcousticModem
//
//  Created by Konstantin Gedalin on 30/09/2017.
//  Copyright © 2017 scifold ltd. All rights reserved.
//

#pragma once

#include <vector>
#include <complex>

const float SC_SAMPLING=44100.0f;
const float scf=1000.0f;

typedef std::complex<float > sci_complex;
typedef std::vector<sci_complex> ComplexVectorD;
typedef std::vector<float>  sci_vector_d;



