/********************************************************************
* CreatedOn: 2007-1-22   21:49
* FileName: CreateDirectoryTree.cpp
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#include <windows.h>
#include <tchar.h>
#include "CreateDirectoryTree.h"
int CreateDirectoryTree(TCHAR* pszDirPath, size_t size)
{
	size_t sizeLen = _tcslen(pszDirPath);
	if (sizeLen > size)
		return -1;

	int nRetCode		= 0;
	DWORD dwAttrs		= INVALID_FILE_ATTRIBUTES;
	TCHAR* pPos			= NULL;
	size_t sizeFillZero = 0;
	pPos = &(pszDirPath[sizeLen - 1]);//从最后一个字符开始

	//先把路径末尾的'\\'号去掉
	while (pPos != pszDirPath && TEXT('\\') == *pPos)
	{
		*pPos = 0;
		--sizeLen;
		--pPos;
		++sizeFillZero;
	}

	if (pPos == pszDirPath && TEXT('\\') == *pPos)
	{
		nRetCode = -1;//路径全为'\\'符号,无效路径名
	}
	else
	{
		dwAttrs = ::GetFileAttributes(pszDirPath);
		if (INVALID_FILE_ATTRIBUTES == dwAttrs)//当前目录不存在
		{
			//寻找最后一个符合条件的\\'号的位置
			//条件是:前一个字符不为'\\'号
			while (pPos != pszDirPath 
				&& (*pPos != TEXT('\\') || TEXT('\\') == *(pPos - 1)))
			{
				--pPos;
			}

			if (*pPos != TEXT('\\'))
			{
				pPos = NULL;//没有找到
			}

			if (pPos != NULL)
			{
				*pPos = 0;//把找到的'\\'号改为终结符号,这样就得到上级目录的路径
				//递归创建上级目录树
				nRetCode = CreateDirectoryTree(pszDirPath, (pPos - pszDirPath + 1));
				*pPos = TEXT('\\');//还原最后一个'\\'号			
			}

			//如果上级目录创建成功或没有上级目录,那么创建当前目录
			if (0 == nRetCode)
			{
				if (!::CreateDirectory(pszDirPath, NULL))
				{
					if (::GetLastError() != ERROR_ALREADY_EXISTS)
						nRetCode = -1;//创建文件夹失败
				}
			}
		}
		else if (!(dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
		{
			nRetCode = -1;//指定的路径存在但不是一个文件夹
		}
		//else当前目录存在,直接返回成功
		//{}
	}

	//如果原来有去掉路径末尾的'\\'号,那么还原回去
	if (sizeFillZero != 0)
	{
		pPos = &(pszDirPath[sizeLen]);
		for (; sizeFillZero != 0; --sizeFillZero, ++pPos)
		{
			*pPos = TEXT('\\');
		}
	}
	return nRetCode;
}