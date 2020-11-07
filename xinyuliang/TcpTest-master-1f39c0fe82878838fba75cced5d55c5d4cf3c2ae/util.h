#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
int32_t encode_int32(char* iBuff, int32_t iMessageLength);
int32_t decode_int32(char* iBuff);
int32_t ip_string_to_addr(const char* string_ip, int32_t& ip);