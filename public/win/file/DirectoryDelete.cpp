/***************************************************************
 * @date:   2007-12-04
 * @author: BrucePeng
 * @brief:  Define function that delete all files in a folder
 */
#include "DirectoryDelete.h"

#include <tchar.h>
#include <stdio.h>

BOOL BrucePengDeleteDirectory(
    IN  TCHAR*      pszDirName, 
    IN  TCHAR*      pszSpecific,
    IN  BOOL        bDeleteSel
)
{
    BOOL    bResult                 = FALSE;
    TCHAR   szSpecName[MAX_PATH]    = {0};
    TCHAR   szFileName[MAX_PATH]    = {0};
    HANDLE  hSearch                 = INVALID_HANDLE_VALUE;

    WIN32_FIND_DATA stResult;

    if(!pszDirName || !pszSpecific)
        goto Exit0;

    //Create specified file name 
    _sntprintf(szSpecName, MAX_PATH, _T("%s\\%s"), 
        pszDirName, pszSpecific
    );

    //Find The first file
    hSearch = FindFirstFile(szSpecName, &stResult);
    if(INVALID_HANDLE_VALUE == hSearch)
        goto Exit0;

    //Delete all files in this DIR
    do 
    {
        _sntprintf(szFileName, MAX_PATH, _T("%s\\%s"), 
            pszDirName, stResult.cFileName
        );

        DeleteFile(szFileName);
    } while(FindNextFile(hSearch, &stResult));

    if(bDeleteSel)
    {
        RemoveDirectory(pszDirName);
    }

    bResult = TRUE;
Exit0:
    if(INVALID_HANDLE_VALUE != hSearch)
        FindClose(hSearch);

    return bResult;
}
