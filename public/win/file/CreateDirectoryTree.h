/********************************************************************
* CreatedOn: 2007-12-3   14:36
* FileName: CreateDirectoryTree.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef CREATEDIRECTORYTREE_H
#define CREATEDIRECTORYTREE_H

/**
 * @brief 递归创建目录树,为了优化效率,使用传入的参数做缓冲区,
 * @remark 可以处理内部有重复斜扛的路径如"C:\\a\\\\b\\\\"
 * @param pszDirPath 目录路径缓冲区
 * @param size 缓冲区大小
 * @return 成功返回0,失败返回负数
 */
int CreateDirectoryTree(TCHAR* pszDirPath, size_t size);


#endif //CREATEDIRECTORYTREE_H