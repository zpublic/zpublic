/*
    Sum 2013/2/21
*/
#ifndef ZL_PEUTILS_DEF_H
#define ZL_PEUTILS_DEF_H

namespace zl
{
namespace Peutils
{
namespace Define
{
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long       uint64;
typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long                int64;
}

typedef enum _PEStatus
{
    PEStatus_Ok,
    PEStatus_Err,
    PEStatus_Unk,
}PEStatus;

typedef enum _PEFileType
{
    PEFILE_NULL,
    PEFILE_R,
    PEFILE_W,
    PEFILE_A,
    PEFILE_ATO_A,
    PEFILE_ATO_R,
    PEFILE_ATO_W,
}PEFileType;

typedef enum _PEFileSeekType
{
    PEFILE_SK_SET,
    PEFILE_SK_CUR,
    PEFILE_SK_END,
}PEFileSeekType;

}
}

#endif