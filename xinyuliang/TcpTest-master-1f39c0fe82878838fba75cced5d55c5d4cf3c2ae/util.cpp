#include "util.h"
#include <iostream>

using namespace std;

int32_t encode_int32(char* iBuff, int32_t iMessageLength) {
    char bytes[4];
        bytes[0] = (iMessageLength >> 24) & 0xFF;
        bytes[1] = (iMessageLength >> 16) & 0xFF;
        bytes[2] = (iMessageLength >> 8) & 0xFF;
        bytes[3] = (iMessageLength) & 0xFF;
    memcpy(iBuff, bytes, sizeof(bytes));
    return sizeof(int32_t);
}

int32_t decode_int32(char* iBuff) {
    int num = 0;
        for (int i = 0; i < 4; i++) {
            num << 8;
            num |= iBuff[i];
       }
    
    return num;
}
int32_t ip_string_to_addr(const char* string_ip, int32_t& ip) {
    struct in_addr stAddr;
    if (!string_ip || inet_pton(AF_INET, string_ip, &stAddr) <= 0) {
        return -1;
    }
    ip = (int32_t)stAddr.s_addr;
    return 0;
}
