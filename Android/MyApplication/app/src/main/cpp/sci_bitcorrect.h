//
//  sci_bitcorrect.h

//
//  Created by konstantin gedalin on 4/20/12.
//  Copyright (c) 2012 Frodo. All rights reserved.
//


#pragma once

#include "sci_base_def.h"
#include "sci_rs.h"
using namespace std;


namespace sci_modem
{
    namespace sci_common{
        vector<uint8_t> Hex2Bin(std::string hexString);
        std::string  create_rs(std::string hexString);
    };
};

