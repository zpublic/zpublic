/***************************************************************
 * @date:   2007-12-04
 * @author: BrucePeng
 * @brief:  Define function that delete all files in a folder
 */
#ifndef DIRECTORY_DELETE_H_
#define DIRECTORY_DELETE_H_

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 删除指定目录中的文件
 *
 * @param[IN]   pszDirName  目录名字(路径)
 * @param[IN]   pszSpecific The specific name
 * @param[IN]   bDeleteSelf 是否删除目录文件夹自身
 *
 * @note
 * 此函数不支持第归删除，即不能删除子目录文件夹中的文件
 *
 * @return 是否成功
 * -TRUE    成功
 *  FALSE   失败
 */
BOOL BrucePengDeleteDirectory(
    IN  TCHAR*  pszDirName, 
    IN  TCHAR*  pszSpecific,
    IN  BOOL    bDeleteSelf
);

#ifdef __cplusplus
}
#endif

#endif//DIRECTORY_DELETE_H_
