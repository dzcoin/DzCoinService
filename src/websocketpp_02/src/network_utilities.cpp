﻿/*
 * copyright (c) 2011, peter thorson. all rights reserved.
 *
 * redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * neither the name of the websocket++ project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * this software is provided by the copyright holders and contributors "as is" 
 * and any express or implied warranties, including, but not limited to, the 
 * implied warranties of merchantability and fitness for a particular purpose 
 * are disclaimed. in no event shall peter thorson be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused and
 * on any theory of liability, whether in contract, strict liability, or tort
 * (including negligence or otherwise) arising in any way out of the use of this
 * software, even if advised of the possibility of such damage.
 * 
 */

#include "network_utilities.hpp"



uint64_t zsutil::htonll(uint64_t src) { 
    static int typ = typ_init; 
    unsigned char c; 
    union { 
        uint64_t ull; 
        unsigned char c[8]; 
    } x; 
    if (typ == typ_init) { 
        x.ull = 0x01; 
        typ = (x.c[7] == 0x01ull) ? typ_bige : typ_smle; 
    } 
    if (typ == typ_bige) 
        return src; 
    x.ull = src; 
    c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c; 
    c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c; 
    c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c; 
    c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c; 
    return x.ull; 
}

uint64_t zsutil::ntohll(uint64_t src) { 
    return htonll(src);
}

std::string zsutil::lookup_ws_close_status_string(uint16_t code) {
    switch (code) {
        case 1000: 
            return "normal closure";
        case 1001: 
            return "going away";
        case 1002: 
            return "protocol error";
        case 1003: 
            return "unacceptable data";
        case 1004: 
            return "reserved";
        case 1005: 
            return "no status received";
        case 1006: 
            return "abnormal closure";
        case 1007: 
            return "invalid message data";
        case 1008: 
            return "policy violation";
        case 1009: 
            return "message too large";
        case 1010: 
            return "missing required extensions";
        case 1011: 
            return "internal server error";
        default:
            return "unknown";
    }
}

std::string zsutil::to_hex(const std::string& input) {
    std::string output;
    std::string hex = "0123456789abcdef";
    
    for (size_t i = 0; i < input.size(); i++) {
        output += hex[(input[i] & 0xf0) >> 4];
        output += hex[input[i] & 0x0f];
        output += " ";
    }
    
    return output;
}

std::string zsutil::to_hex(const char* input,size_t length) {
    std::string output;
    std::string hex = "0123456789abcdef";
    
    for (size_t i = 0; i < length; i++) {
        output += hex[(input[i] & 0xf0) >> 4];
        output += hex[input[i] & 0x0f];
        output += " ";
    }
    
    return output;
}


