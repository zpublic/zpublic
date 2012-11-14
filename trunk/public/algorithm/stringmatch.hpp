#ifndef STRINGMATCH_HPP__
#define STRINGMATCH_HPP__

namespace zl
{
#include <stdlib.h>
#include <stdio.h>

int SundayMatchString(const char* lpSource, int nSourceLen, const char* lpPattern, int nPatLen, int flag)
{
	int			nStep[256];
	int			nPos		= 0;
	int			nLen;
	int			j;

	if(lpSource == NULL || lpPattern == NULL)
	{
		return -1;
	}


	if(nPatLen > nSourceLen)
	{
		return -1;
	}
	for(int i=0; i<256; i++)
	{
		nStep[i] = nPatLen + 1;
	}

	//构造pattern数组;
	for(int i=0; i<nPatLen; i++)
	{
		nStep[(unsigned char)lpPattern[i]] = nPatLen - i; 	
	}

	nLen = 0;
	while(nPos < nSourceLen-nPatLen+1)
	{
		if(lpSource[nPos] != lpPattern[nLen])
		{
			//第一个位置不相等 比较紧跟字串的最后一个字符;
			if(nPos+nPatLen > nSourceLen)
				break;
			nPos +=  nStep[(unsigned char)lpSource[nPos+nPatLen]];

		}
		else
		{	
			for(j=1; j < nPatLen; j++)
			{
				if(flag && *(lpPattern+j) == '*')
					continue;

				if(*(lpSource+nPos+j) != *(lpPattern+j))
				{
					break;
				}
			}
			if(j == nPatLen)
			{
				return nPos;
			}

			nPos++;
		}
	}

	return -1;
}

}
#endif // STRINGMATCH_HPP__
