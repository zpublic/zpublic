///////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StrSim.cpp
//  Version     :   1.0
//  Creater     :   Zhao Min
//  Date        :   2006-1-22
//  Comment     :   
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StrSim.h"

//////////////////////////////////////////////////////////////////////////
#define MAX_STR_SIZE 33

#define CASE_GET_REPALCE(firc,secc) \
	case ((firc<<8) | secc):\
	case ((secc<<8) | firc):\

#define CASE_GET_REPALCE_SCORE(firc,secc,score) \
	case ((firc<<8) | secc):\
	case ((secc<<8) | firc):\
		nResult = score;\
		break
//////////////////////////////////////////////////////////////////////////
typedef struct _SIM_RECORD
{
    int nSim;
    int nIndex;
	
}SIM_RECORD, *P_SIM_RECORD;
//////////////////////////////////////////////////////////////////////////
inline int GetReplaceScore(LPCSTR lpszFirChar,LPCSTR lpszSecChar);
inline int GetDeleteScore(LPCSTR lpszChar);
//////////////////////////////////////////////////////////////////////////

int StrSim(
           LPCSTR		lpszFirStr,
           const int	nFirStrLen,
           LPCSTR		cSecStr,
           const int	nSecStrLen
)
{
    int nResult  = 0;
    int nRetCode = 0;
    int table[MAX_STR_SIZE + 1][MAX_STR_SIZE + 1] = {0};
    int i = 0, j = 0;
    int nScoreLeft = 0,nScoreTop = 0, nScoreTopLeft = 0;
    
    if(
        nFirStrLen > MAX_STR_SIZE ||
        nSecStrLen > MAX_STR_SIZE ||
        nFirStrLen < 1 ||
        nSecStrLen < 1
    )
        goto Exit0;


    for (i = 0; i <= nFirStrLen; ++i)
        table[i][0] = (-i * 100);

    for (j = 0; j <= nSecStrLen; ++j)
        table[0][j] = (-j * 100);

    for(i = 0; i < nFirStrLen; ++i)
        for(j = 0; j < nSecStrLen; ++j)
        {
            if(0 == memicmp((void*)&lpszFirStr[i], (void*)&cSecStr[j], sizeof(TCHAR)))
                nScoreTopLeft = table[i][j] + 100;//if match
            else
            {//Replace
                // case exchange at least add 50; 
                if(// [a]b<-->[b]a
						(j + 1 < nSecStrLen) &&
						(0 == memicmp((void*)&lpszFirStr[i], (void*)&cSecStr[j+1], sizeof(TCHAR))) &&
						(i + 1 < nFirStrLen) &&
						0 == memicmp((void*)&lpszFirStr[j], (void*)&cSecStr[i+1], sizeof(TCHAR))
                   )
                    nScoreTopLeft = 
                        table[i][j] +max(50,GetReplaceScore(&lpszFirStr[i],&cSecStr[j]));
                
                else if (// a[b] <--> b[a]                   
						(j - 1  >= 0) &&
						(0 == memicmp((void*)&lpszFirStr[i],(void*)&cSecStr[j-1], sizeof(TCHAR))) &&
						(i - 1 >= 0)  &&
						(0 == memicmp((void*)&lpszFirStr[j],(void*)&cSecStr[i-1], sizeof(TCHAR)))
                    )
                    nScoreTopLeft = 
                        table[i][j] + max(50, GetReplaceScore(&lpszFirStr[i],&cSecStr[j]));

                else //Normal
                    nScoreTopLeft = 
                        table[i][j] + GetReplaceScore(&lpszFirStr[i],&cSecStr[j]);
            }

            nScoreLeft = table[i][j+1] + GetDeleteScore(&lpszFirStr[i]);//Insert
            nScoreTop  = table[i+1][j] + GetDeleteScore(&cSecStr[j]);//delete

            //get the Max;
            table[i+1][j+1] = max(
                max(nScoreLeft,nScoreTop),
                nScoreTopLeft
            );
            //if(0 == j)
            //    printf("\n");
            //printf("%4d",table[i+1][j+1]);            
        }
    // Get Sim
    nResult = 2 * table[nFirStrLen][nSecStrLen] / (nFirStrLen + nSecStrLen);

Exit0:
    return nResult;
}
inline int GetDeleteScore(LPCSTR FirChar)
{
    int nResult = 0;
    switch(*FirChar)
    {
    case ' ':
        nResult = -10;
        break;
    case '3':
    case '2':
        nResult = -10;
        break;
    case '6':
    case '4':
        nResult = -20;
        break;
    default:
        nResult = -50;
    }
    return nResult;
}


inline int GetReplaceScore(LPCSTR lpszFirChar,LPCSTR lpszSecChar)
{
    int nResult = 0;
    switch((*lpszFirChar << 8) | *lpszSecChar)
    {
        CASE_GET_REPALCE('0','o')
        CASE_GET_REPALCE('1','l')
        CASE_GET_REPALCE('I','l')
        CASE_GET_REPALCE('I','1')
        CASE_GET_REPALCE('8','B')
        CASE_GET_REPALCE_SCORE('0','O',95);
        CASE_GET_REPALCE_SCORE('a','e',50);
        CASE_GET_REPALCE_SCORE('v','y',65);
        CASE_GET_REPALCE_SCORE('v','u',80);
    default:
        nResult = 0;

    }
    return nResult;
}
int CheckSimExeFileNameInList(
        LPCSTR cszPathName,
        int nSimilarity,
        LPCSTR  cszSimNameList [],
        int nNameListSize,
        BOOL bRet100
)
{
	int nResult = 0;
    int nRetCode = 0;
	char cszFileName[MAX_PATH] = {0};
	const char *cszPtr = NULL;
    char *pFileNameExt = NULL;
    const char *pBlankNameExt = NULL;
    SIM_RECORD BigRecord[2];
    BigRecord[0].nSim = 0;
    BigRecord[0].nIndex = 0;
    BigRecord[1].nSim = 0;
    BigRecord[1].nIndex = 0;
    int nBlankNameLen;
    int nFileNameLen;
	int i = 0;
    int j = 0;
	cszPtr = strrchr(cszPathName, '\\');
	if (cszPtr != NULL)
	{
		strcpy(cszFileName, cszPtr + 1);
	}
	else
	{
		strcpy(cszFileName, cszPathName);
	}

    pFileNameExt = strrchr(cszFileName,'.');
    if(pFileNameExt != NULL)
    {
        if(     
            !(
            0 == stricmp(pFileNameExt,".exe")||
            0 == stricmp(pFileNameExt,".dll")||
            0 == stricmp(pFileNameExt,".scr")||
            0 == stricmp(pFileNameExt,".pif")||
            0 == stricmp(pFileNameExt,".com")
            )
        )
            goto Exit0;
    }
	for (i = 0; i < nNameListSize; i++)
	{
        nFileNameLen = strlen(cszFileName);
        pBlankNameExt = strrchr(cszSimNameList[i],'.');
        nBlankNameLen = strlen(cszSimNameList[i]);
        
        //process Extent(.exe)
        //如果文件名和列表中的扩展名相同则比较扩展名
        //如果列表中的扩展名为.dll 也不对比扩展名
        if(pFileNameExt != NULL && pBlankNameExt != NULL)
        {
            if(
                0 == stricmp(pFileNameExt,pBlankNameExt) ||
                0 == stricmp(pBlankNameExt,".dll")
            )
            {
                nFileNameLen  -= strlen(pFileNameExt);
                if(nFileNameLen <=3)
                    goto Exit0;
                nBlankNameLen -= strlen(pBlankNameExt);
            }
            pBlankNameExt = NULL;
        }
        
        //计算
		nRetCode = StrSim(cszSimNameList[i],nBlankNameLen,cszFileName,nFileNameLen);
        
        //如果值为100 且要求返回100的值
        if(100 == nRetCode && bRet100)
        {
            nResult = nRetCode;
            goto Exit0;
        }
        //记录列表中与文件名最相近的两个文件名
        if(BigRecord[0].nSim < nRetCode)
        {
            BigRecord[1] = BigRecord[0];
            BigRecord[0].nSim = nRetCode;
            BigRecord[0].nIndex = i;
        }
        else if(BigRecord[1].nSim < nRetCode)
        {
            BigRecord[1].nSim = nRetCode;
            BigRecord[1].nIndex = i;            
        }

	}



    if(BigRecord[0].nSim < nSimilarity)
    {//最大相似度小于要求
        if(BigRecord[0].nSim + BigRecord[1].nSim > nSimilarity)
        {//最大的两个值之和大于相似度的一半，即该字符可能由列表中的两个串拼接而成
            i = BigRecord[0].nIndex;
            j = BigRecord[1].nIndex;
            nRetCode = StrSim(
                cszSimNameList[i],
                strlen(cszSimNameList[i])-4,
                cszSimNameList[j],
                strlen(cszSimNameList[j])-4
            );
            if(BigRecord[1].nSim > nRetCode)
            {
                nResult = (BigRecord[0].nSim + (BigRecord[1].nSim - nRetCode) / 3) % 100;
            }
        }

    }
    else
    {//最大相似度大于要求
       nResult = BigRecord[0].nSim;
    }
Exit0:
    if(nResult < nSimilarity)
        nResult = 0;
    if(!bRet100 && nResult == 100)
        nResult = 0;
	return nResult;
}

//inline int GetReplaceScore(const char *pChar)
//{
//    nResult = 0;
//    return 0;
//}
//int main(int argc, char* argv[])
//{
//    int nSim;
//    char cTest1[MAX_STR_SIZE -1]="svthost",
//        cTest2[MAX_STR_SIZE -1]= "svch0st";
//    nSim = strsim(cTest1,cTest2);
//        printf("\n%d\n",nSim);
//	return 0;
//}