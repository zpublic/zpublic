///////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StrSim.h
//  Version     :   1.0
//  Creater     :   Zhao Min
//  Date        :   2006-1-22
//  Comment     :   
//
///////////////////////////////////////////////////////////////////////////////
int StrSim(
           LPCSTR		lpszFirStr,
           const int	nFirStrLen,
           LPCSTR		cSecStr,
           const int	nSecStrLen
);


int CheckSimExeFileNameInList(
        LPCSTR cszPathName,
        int		nSimilarity,
        LPCSTR cszSimNameList [],
        int nNameListSize,
        BOOL bRet100
);