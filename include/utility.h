#pragma once    
#include <stdio.h>
#define BASE_LOG(fmt,info, ...) printf(info fmt "\n\r", ##__VA_ARGS__);
#if DEBUG
    #define LOG_DBG(fmt, ...) BASE_LOG(fmt,"[Debug]: ", ##__VA_ARGS__);
#else
    #define LOG_DBG(fmt, ...) do {} while(0);
#endif
#define LOG_INF(fmt, ...) BASE_LOG(fmt,"[Info]: ", ##__VA_ARGS__);
#define LOG_ERR(fmt, ...) BASE_LOG(fmt,"[Error]: ", ##__VA_ARGS__);
#define UNUSED(x) (void)(x);
