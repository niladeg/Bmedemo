//
//  rs.h
//  QPSKLib
//
//  Created by konstantin gedalin on 8/14/12.
//  Copyright (c) 2012 Frodo. All rights reserved.
//

#pragma once
#include <iostream>
#include <vector>


#define kGFAlpha  4
#define kCodeWordLength  15
#define kNumberOfErrorsToCorrect   3                 // number of errors that can correcte
#define kNumberOfDataBits       9
namespace sci_modem
{
    namespace sci_common
    {
    class rs
    {
        
        int elp[kCodeWordLength-kNumberOfDataBits+2][kCodeWordLength-kNumberOfDataBits]={0};
        int d[kCodeWordLength-kNumberOfDataBits+2]={0};
        int l[kCodeWordLength-kNumberOfDataBits+2]={0};
        int u_lu[kCodeWordLength-kNumberOfDataBits+2]={0};
        int s[kCodeWordLength-kNumberOfDataBits+1]={0};
        int root[kNumberOfErrorsToCorrect]={0};
        int loc[kNumberOfErrorsToCorrect]={0};//error colcation
        int z[kNumberOfErrorsToCorrect+1]={0};
        int err[kCodeWordLength]={0};
        int reg[kNumberOfErrorsToCorrect+1]={0} ;
        int polynomialCoeffs [kGFAlpha+1] = { 1, 0, 0, 1, 1} ; /* specify irreducible polynomial coeffts */
        int alpha_to [kCodeWordLength+1]={0};
        int index_of [kCodeWordLength+1]={0};
        int primitive[kCodeWordLength-kNumberOfDataBits+1]={0};
        
    public:
        
        
        enum{
        NO_ERRORS=0,
        ERROR_EXIST,
        TOO_MUCH_ERRORS,
        ERRORS_CORRECTED,
        NUM_OF_REED_SOLOMON_ERR_CODES
        };
        
        
        rs();
        ~rs(){};
        int prepare(std::vector<uint8_t> bindataIn);
        void generate_gf();
        void generator_polynomial();
        
        ;
        void encode_rs(std::vector<int> &data,std::vector<int> &bb);
        int  decode_rs(std::vector<int> &recd);
        int  Decode(std::vector<int> &recd);
    private:
        void UpdateRecordedData(std::vector<int> &recd);
        int  FormSyndromes(std::vector<int> &recd);
        int  computeerors();
        int  calculateroots(int u);
    public:
        int iLasterrorNum;
        
    };
    };
};



