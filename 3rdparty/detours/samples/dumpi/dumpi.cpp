//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (dumpi.cpp of dumpi.exe)
//
//  Microsoft Research Detours Package, Version 3.0.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shellapi.h>
#include <detours.h>

////////////////////////////////////////////////////////////// Error Messages.
//
VOID AssertMessage(PCSTR szMsg, PCSTR szFile, DWORD nLine)
{
    printf("ASSERT(%s) failed in %s, line %d.", szMsg, szFile, nLine);
}

#define ASSERT(x)   \
do { if (!(x)) { AssertMessage(#x, __FILE__, __LINE__); DebugBreak(); }} while (0)
    ;

//////////////////////////////////////////////////////////////////////////////
//
static BOOL s_fSubs = FALSE;

//////////////////////////////////////////////////////////////////////////////
//
static CHAR s_szFile[MAX_PATH] = "\0";

static BOOL CALLBACK ListFileCallback(PVOID pContext,
                                      PCHAR pszOrigFile,
                                      PCHAR pszFile,
                                      PCHAR *ppszOutFile)
{
    (void)pContext;
    (void)pszFile;

    *ppszOutFile = NULL;

#ifdef _CRT_INSECURE_DEPRECATE
    strcpy_s(s_szFile, sizeof(s_szFile), pszOrigFile);
#else
    strcpy(s_szFile, pszOrigFile);
#endif

    PCHAR psz;
    if ((psz = strchr(s_szFile, '.')) != NULL) {
        *psz = '\0';
    }
    return TRUE;
}

BOOL CALLBACK ListSymbolCallback(PVOID pContext,
                                 ULONG nOrigOrdinal,
                                 ULONG nOrdinal,
                                 ULONG *pnOutOrdinal,
                                 PCHAR pszOrigSymbol,
                                 PCHAR pszSymbol,
                                 PCHAR *ppszOutSymbol)
{
    (void)pContext;
    (void)nOrdinal;
    (void)pszSymbol;

    *ppszOutSymbol = NULL;
    *pnOutOrdinal = 0;

    if (nOrigOrdinal != 0) {
        printf("  %s::#%d\n",
               s_szFile, nOrigOrdinal);
    }
    else {
        printf("  %s::%s\n",
               s_szFile, pszOrigSymbol);
    }

    return TRUE;
}

BOOL DimpFile(PCHAR pszPath)
{
    BOOL bGood = TRUE;
    HANDLE hOld = INVALID_HANDLE_VALUE;
    PDETOUR_BINARY pBinary = NULL;


    hOld = CreateFile(pszPath,
                      GENERIC_READ,
                      FILE_SHARE_READ,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);

    if (hOld == INVALID_HANDLE_VALUE) {
        printf("%s: Failed to open input file with error: %d\n",
               pszPath, GetLastError());
        bGood = FALSE;
        goto end;
    }

    if ((pBinary = DetourBinaryOpen(hOld)) == NULL) {
        printf("%s: DetourBinaryOpen failed: %d\n", pszPath, GetLastError());
        goto end;
    }

    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }

    printf("%s:\n", pszPath);
    if (!DetourBinaryEditImports(pBinary,
                                 NULL,
                                 NULL,
                                 ListFileCallback,
                                 ListSymbolCallback,
                                 NULL)) {

        printf("%s: DetourBinaryEditImports failed: %d\n", pszPath, GetLastError());
    }

    DetourBinaryClose(pBinary);
    pBinary = NULL;

  end:
    if (pBinary) {
        DetourBinaryClose(pBinary);
        pBinary = NULL;
    }
    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }
    return bGood;
}

//////////////////////////////////////////////////////////////////////////////
int DimpArgument(char *dir, char *argp, int fDoSubs)
{
    //////////////////////////////////////////////////////////////////////////

    WIN32_FIND_DATA wfd;
    HANDLE  hFind = NULL;
    char    name[1024];
    int     nFound = 0;

#ifdef _CRT_INSECURE_DEPRECATE
    strcpy_s(name, sizeof(name), dir ? dir : "");
    strcat_s(name, sizeof(name), argp);
#else
    strcpy(name, dir ? dir : "");
    strcat(name, argp);
#endif

    hFind = FindFirstFile(name, &wfd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
#ifdef _CRT_INSECURE_DEPRECATE
                strcpy_s(name, sizeof(name), dir ? dir : "");
                strcat_s(name, sizeof(name), wfd.cFileName);
#else
                strcpy(name, dir ? dir : "");
                strcat(name, wfd.cFileName);
#endif

                nFound += DimpFile(name);
            }
        } while (FindNextFile(hFind, &wfd));
        FindClose(hFind);
    }

    if (fDoSubs) {
#ifdef _CRT_INSECURE_DEPRECATE
        strcpy_s(name, sizeof(name), dir ? dir : "");
        strcat_s(name, sizeof(name), "*");
#else
        strcpy(name, dir ? dir : "");
        strcat(name, "*");
#endif

        hFind = FindFirstFile(name, &wfd);
        if (hFind == INVALID_HANDLE_VALUE)
            return nFound;

        do {
            if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                wfd.cFileName[0] != '.') {

#ifdef _CRT_INSECURE_DEPRECATE
                strcpy_s(name, sizeof(name), dir ? dir : "");
                strcat_s(name, sizeof(name), wfd.cFileName);
                strcat_s(name, sizeof(name), "\\");
#else
                strcpy(name, dir ? dir : "");
                strcat(name, wfd.cFileName);
                strcat(name, "\\");
#endif

                nFound += DimpArgument(name, argp, fDoSubs);
            }
        } while (FindNextFile(hFind, &wfd));
        FindClose(hFind);
    }
    return nFound;
}


//////////////////////////////////////////////////////////////////////////////
//
void PrintUsage(void)
{
    printf("Usage:\n"
           "    dimp [options] binary_files\n"
           "Options:\n"
           "    /s           : Recurse through subdirectories.\n"
           "    /?           : This help screen.\n"
           "Examples:\n"
           "    dimp /s *.exe\n"
           "");
}

//////////////////////////////////////////////////////////////////////// main.
//
int CDECL main(int argc, char **argv)
{
    BOOL fNeedHelp = FALSE;
    BOOL fSubdirs = FALSE;

    int arg = 1;
    for (; arg < argc; arg++) {
        if (argv[arg][0] == '-' || argv[arg][0] == '/') {
            CHAR *argn = argv[arg] + 1;
            CHAR *argp = argn;
            while (*argp && *argp != ':')
                argp++;
            if (*argp == ':')
                *argp++ = '\0';

            switch (argn[0]) {

              case 's':                                 // Do Subdirectories.
              case 'S':
                fSubdirs = TRUE;
                break;

              case '?':                                 // Help.
                fNeedHelp = TRUE;
                break;

              default:
                fNeedHelp = TRUE;
                printf("Bad argument: %s:%s\n", argn, argp);
                break;
            }
        }
        else {
            CHAR szDir[MAX_PATH] = "";
            CHAR szArg[MAX_PATH] = "";
            PCHAR pszDir;

            if ((pszDir = strrchr(argv[arg], '\\')) != NULL) {
                *pszDir++ = '\0';
#ifdef _CRT_INSECURE_DEPRECATE
                strcpy_s(szArg, sizeof(szArg), pszDir);
                strcpy_s(szDir, sizeof(szDir), argv[arg]);
                strcat_s(szDir, sizeof(szDir), "\\");
#else
                strcpy(szArg, pszDir);
                strcpy(szDir, argv[arg]);
                strcat(szDir, "\\");
#endif
            }
            else {
                if (GetCurrentDirectory(sizeof(szDir), szDir) > 3) {
#ifdef _CRT_INSECURE_DEPRECATE
                    strcat_s(szDir, sizeof(szDir), "\\");
#else
                    strcat(szDir, "\\");
#endif
                }
#ifdef _CRT_INSECURE_DEPRECATE
                strcpy_s(szArg, sizeof(szArg), argv[arg]);
#else
                strcpy(szArg, argv[arg]);
#endif
            }

            DimpArgument(szDir, szArg, fSubdirs);
        }
    }
    if (argc == 1) {
        fNeedHelp = TRUE;
    }
    if (fNeedHelp) {
        PrintUsage();
        return 1;
    }
    return 0;
}

// End of File
