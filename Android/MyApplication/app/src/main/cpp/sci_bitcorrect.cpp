//
//  sci_bitcorrect.cpp
//  QPSKLib
//
//  Created by konstantin gedalin on 4/20/12.
//  Copyright (c) 2012 Frodo. All rights reserved.
//
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "sci_rs.h"
#include "sci_bitcorrect.h"
using namespace std;

using namespace sci_modem::sci_common;

namespace sci_modem
{
    namespace sci_common{
        

        
        int scanner(string val)
        {
            if(!val.compare("A") || !val.compare("a"))
                return 10;
            if(!val.compare("B") || !val.compare("b") )
                return 11;
            if(!val.compare("C") || !val.compare("c"))
                return 12;
            if(!val.compare("D") || !val.compare("d"))
                return 13;
            if(!val.compare("E") || !val.compare("e"))
                return 14;
            if(!val.compare("F") || !val.compare("f"))
                return 15;
            return atoi(val.c_str());
        }
        vector<uint8_t> Hex2Bin(std::string hexString)
        {
            vector<uint8_t> binNum;
            int sa=(int)hexString.length();
            for (int i = 0; i < (int)sa*4; i+=4)
            {
                int length = 1;
                int location = (int)(i/4);
                string currentString = hexString.substr(location,length);
                int hexValue=scanner(currentString);
                int bit1 = (hexValue & 0x1);
                int bit2 = (hexValue & 0x2) >> 1;
                int bit3 = (hexValue & 0x4) >> 2;
                int bit4 = (hexValue & 0x8) >> 3;
                binNum.push_back(bit4);
                binNum.push_back(bit3);
                binNum.push_back(bit2);
                binNum.push_back(bit1);
                
            }
            return binNum;
        }
        
        std::string  create_rs(std::string hexString)
        {
            rs rs;
            vector<uint8_t> binData = Hex2Bin(hexString);
            int res=rs.prepare(binData);
            std::stringstream sstream;
            sstream << setfill('0') << setw(6) << std::hex << res;
            std::string OutHex = sstream.str();
             OutHex=hexString+OutHex;
            return OutHex;
        }
        
        
        
    };
};
