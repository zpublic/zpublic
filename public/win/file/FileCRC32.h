#ifndef FILE_CRC32_H_
#define FILE_CRC32_H_

#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

bool GetFileCRC32(const TCHAR* pszFileName, unsigned& uCrc32);

bool GetFileCRC32Ex(const TCHAR* pszFileName, unsigned& uCrc32);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//FILE_CRC32_H_
