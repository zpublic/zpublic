/********************************************************************
* @file      : base64.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/18 13:39
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _BASE64_HPP_
#define _BASE64_HPP_

#include "../../3rdparty/modp_b64/modp_b64.h"

namespace zl
{

    static bool Base64Encode(const std::string& input, std::string* output)
    {
        std::string temp;
        temp.resize(modp_b64_encode_len(input.size()));

        int input_size = static_cast<int>(input.size());

        int output_size = modp_b64_encode(&(temp[0]), input.data(), input_size);
        if (output_size < 0)
            return false;

        temp.resize(output_size);
        output->swap(temp);
        return true;
    }

    static bool Base64Decode(const std::string& input, std::string* output)
    {
        std::string temp;
        temp.resize(modp_b64_decode_len(input.size()));

        int input_size = static_cast<int>(input.size());
        int output_size = modp_b64_decode(&(temp[0]), input.data(), input_size);
        if (output_size < 0)
            return false;

        temp.resize(output_size);
        output->swap(temp);
        return true;
    }

}

#endif