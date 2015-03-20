#pragma once

typedef char            zl_char;
typedef unsigned char   zl_uchar;
typedef char            zl_int8;
typedef unsigned char   zl_uint8;
typedef short           zl_int16;
typedef unsigned short  zl_uint16;
typedef int             zl_int32;
typedef unsigned int    zl_uint32;

#ifdef Z_PLATFORM_WIN
typedef __int64             zl_int64;
typedef unsigned __int64    zl_uint64;
#else
typedef long long           zl_int64;
typedef unsigned long long  zl_uint64;
#endif
