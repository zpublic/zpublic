//////////////////////////////////////////////////////////////////////////////
//
//  Test DetourCreateProcessWithDll function (withdll.cpp).
//
//  Microsoft Research Detours Package, Version 3.0.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#include <stdio.h>
#include <windows.h>
#include <detours.h>

//////////////////////////////////////////////////////////////////////////////
//
void PrintUsage(void)
{
    printf("Usage:\n"
           "    withdll.exe [options] [command line]\n"
           "Options:\n"
           "    /d:file.dll   : Start the process with file.dll.\n"
           "    /v            : Verbose, display memory at start.\n"
           "    /?            : This help screen.\n");
}

//////////////////////////////////////////////////////////////////////////////
//
//  This code verifies that the named DLL has been configured correctly
//  to be imported into the target process.  DLLs must export a function with
//  ordinal #1 so that the import table touch-up magic works.
//
struct ExportContext
{
    BOOL    fHasOrdinal1;
    ULONG   nExports;
};

static BOOL CALLBACK ExportCallback(PVOID pContext,
                                    ULONG nOrdinal,
                                    PCHAR pszSymbol,
                                    PVOID pbTarget)
{
    (void)pContext;
    (void)pbTarget;
    (void)pszSymbol;

    ExportContext *pec = (ExportContext *)pContext;

    if (nOrdinal == 1) {
        pec->fHasOrdinal1 = TRUE;
    }
    pec->nExports++;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////////
//

void TypeToString(DWORD Type, char *pszBuffer, size_t cBuffer)
{
    if (Type == MEM_IMAGE) {
        sprintf_s(pszBuffer, cBuffer, "img");
    }
    else if (Type == MEM_MAPPED) {
        sprintf_s(pszBuffer, cBuffer, "map");
    }
    else if (Type == MEM_PRIVATE) {
        sprintf_s(pszBuffer, cBuffer, "pri");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", Type);
    }
}

void StateToString(DWORD State, char *pszBuffer, size_t cBuffer)
{
    if (State == MEM_COMMIT) {
        sprintf_s(pszBuffer, cBuffer, "com");
    }
    else if (State == MEM_FREE) {
        sprintf_s(pszBuffer, cBuffer, "fre");
    }
    else if (State == MEM_RESERVE) {
        sprintf_s(pszBuffer, cBuffer, "res");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", State);
    }
}

void ProtectToString(DWORD Protect, char *pszBuffer, size_t cBuffer)
{
    if (Protect == 0) {
        sprintf_s(pszBuffer, cBuffer, "");
    }
    else if (Protect == PAGE_EXECUTE) {
        sprintf_s(pszBuffer, cBuffer, "--x");
    }
    else if (Protect == PAGE_EXECUTE_READ) {
        sprintf_s(pszBuffer, cBuffer, "r-x");
    }
    else if (Protect == PAGE_EXECUTE_READWRITE) {
        sprintf_s(pszBuffer, cBuffer, "rwx");
    }
    else if (Protect == PAGE_EXECUTE_WRITECOPY) {
        sprintf_s(pszBuffer, cBuffer, "rcx");
    }
    else if (Protect == PAGE_NOACCESS) {
        sprintf_s(pszBuffer, cBuffer, "---");
    }
    else if (Protect == PAGE_READONLY) {
        sprintf_s(pszBuffer, cBuffer, "r--");
    }
    else if (Protect == PAGE_READWRITE) {
        sprintf_s(pszBuffer, cBuffer, "rw-");
    }
    else if (Protect == PAGE_WRITECOPY) {
        sprintf_s(pszBuffer, cBuffer, "rc-");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE)) {
        sprintf_s(pszBuffer, cBuffer, "g--x");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_READ)) {
        sprintf_s(pszBuffer, cBuffer, "gr-x");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_READWRITE)) {
        sprintf_s(pszBuffer, cBuffer, "grwx");
    }
    else if (Protect == (PAGE_GUARD | PAGE_EXECUTE_WRITECOPY)) {
        sprintf_s(pszBuffer, cBuffer, "grcx");
    }
    else if (Protect == (PAGE_GUARD | PAGE_NOACCESS)) {
        sprintf_s(pszBuffer, cBuffer, "g---");
    }
    else if (Protect == (PAGE_GUARD | PAGE_READONLY)) {
        sprintf_s(pszBuffer, cBuffer, "gr--");
    }
    else if (Protect == (PAGE_GUARD | PAGE_READWRITE)) {
        sprintf_s(pszBuffer, cBuffer, "grw-");
    }
    else if (Protect == (PAGE_GUARD | PAGE_WRITECOPY)) {
        sprintf_s(pszBuffer, cBuffer, "grc-");
    }
    else {
        sprintf_s(pszBuffer, cBuffer, "%x", Protect);
    }
}

static BYTE buffer[65536];

typedef union
{
    struct
    {
        DWORD Signature;
        IMAGE_FILE_HEADER FileHeader;
    } ih;

    IMAGE_NT_HEADERS32 ih32;
    IMAGE_NT_HEADERS64 ih64;
} IMAGE_NT_HEADER;

struct SECTIONS
{
    PBYTE   pbBeg;
    PBYTE   pbEnd;
    CHAR    szName[16];
} Sections[256];
DWORD SectionCount = 0;
DWORD Bitness = 0;

PCHAR FindSectionName(PBYTE pbBase, PBYTE& pbEnd)
{
    for (DWORD n = 0; n < SectionCount; n++) {
        if (Sections[n].pbBeg == pbBase) {
            pbEnd = Sections[n].pbEnd;
            return Sections[n].szName;
        }
    }
    pbEnd = NULL;
    return NULL;
}

ULONG PadToPage(ULONG Size)
{
    return (Size & 0xfff)
        ? Size + 0x1000 - (Size & 0xfff)
        : Size;
}

BOOL GetSections(HANDLE hp, PBYTE pbBase)
{
    DWORD beg = 0;
    DWORD cnt = 0;
    SIZE_T done;
    IMAGE_DOS_HEADER idh;

    if (!ReadProcessMemory(hp, pbBase, &idh, sizeof(idh), &done) || done != sizeof(idh)) {
        return FALSE;
    }

    if (idh.e_magic != IMAGE_DOS_SIGNATURE) {
        return FALSE;
    }

    IMAGE_NT_HEADER inh;
    if (!ReadProcessMemory(hp, pbBase + idh.e_lfanew, &inh, sizeof(inh), &done) || done != sizeof(inh)) {
        printf("No Read\n");
        return FALSE;
    }

    if (inh.ih.Signature != IMAGE_NT_SIGNATURE) {
        printf("No NT\n");
        return FALSE;
    }

    beg = idh.e_lfanew
        + FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader )
        + inh.ih.FileHeader.SizeOfOptionalHeader;
    cnt = inh.ih.FileHeader.NumberOfSections;
    Bitness = (inh.ih32.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) ? 32 : 64;
#if 0
    printf("%d %d count=%d\n", beg, Bitness, cnt);
#endif

    IMAGE_SECTION_HEADER ish;
    for (DWORD n = 0; n < cnt; n++) {
        if (!ReadProcessMemory(hp, pbBase + beg + n * sizeof(ish), &ish, sizeof(ish), &done) || done != sizeof(ish)) {
            printf("No Read\n");
            return FALSE;
        }
        Sections[n].pbBeg = pbBase + ish.VirtualAddress;
        Sections[n].pbEnd = pbBase + ish.VirtualAddress + PadToPage(ish.Misc.VirtualSize);
        memcpy(Sections[n].szName, ish.Name, sizeof(ish.Name));
        Sections[n].szName[sizeof(ish.Name)] = '\0';
#if 0
        printf("--- %p %s\n", Sections[n].pbBeg, Sections[n].szName);
#endif
    }
    SectionCount = cnt;

    return TRUE;
}

BOOL DumpProcess(HANDLE hp)
{
    ULONG64 base;
    ULONG64 next;

    MEMORY_BASIC_INFORMATION mbi;

    printf("  %12s %8s %8s: %3s %3s %4s %3s : %8s\n", "Address", "Offset", "Size", "Typ", "Sta", "Prot", "Ini", "Contents");
    printf("  %12s %8s %8s: %3s %3s %4s %3s : %8s\n", "------------", "--------", "--------", "---", "---", "----", "---", "-----------------");

    for (next = 0;;) {
        base = next;
        ZeroMemory(&mbi, sizeof(mbi));
        if (VirtualQueryEx(hp, (PVOID)base, &mbi, sizeof(mbi)) == 0) {
            break;
        }
        if ((mbi.RegionSize & 0xfff) == 0xfff) {
            break;
        }

        next = (ULONG64)mbi.BaseAddress + mbi.RegionSize;

        if (mbi.State == MEM_FREE) {
            continue;
        }

        CHAR szType[16];
        TypeToString(mbi.Type, szType, ARRAYSIZE(szType));
        CHAR szState[16];
        StateToString(mbi.State, szState, ARRAYSIZE(szState));
        CHAR szProtect[16];
        ProtectToString(mbi.Protect, szProtect, ARRAYSIZE(szProtect));
        CHAR szAllocProtect[16];
        ProtectToString(mbi.AllocationProtect, szAllocProtect, ARRAYSIZE(szAllocProtect));

        CHAR szFile[MAX_PATH];
        szFile[0] = '\0';
        DWORD cb = 0;
        PCHAR pszFile = szFile;

        if (base == (ULONG64)mbi.AllocationBase) {
#if 0
            cb = pfGetMappedFileName(hp, (PVOID)mbi.AllocationBase, szFile, ARRAYSIZE(szFile));
#endif
            if (GetSections(hp, (PBYTE)mbi.AllocationBase)) {
                next = base + 0x1000;
                sprintf_s(szFile, ARRAYSIZE(szFile), "%d-bit PE", Bitness);
            }
        }
        if (cb > 0) {
            for (DWORD c = 0; c < cb; c++) {
                szFile[c] = (szFile[c] >= 'a' && szFile[c] <= 'z')
                    ? szFile[c] - 'a' + 'A' : szFile[c];
            }
            szFile[cb] = '\0';
        }

        if ((pszFile = strrchr(szFile, '\\')) == NULL) {
            pszFile = szFile;
        }
        else {
            pszFile++;
        }

        PBYTE pbEnd;
        PCHAR pszSect = FindSectionName((PBYTE)base, pbEnd);
        if (pszSect != NULL) {
            pszFile = pszSect;
            if (next > (ULONG64)pbEnd) {
                next = (ULONG64)pbEnd;
            }
        }

        CHAR szDesc[128];
        ZeroMemory(&szDesc, ARRAYSIZE(szDesc));
        if (base == (ULONG64)mbi.AllocationBase) {
            sprintf_s(szDesc, ARRAYSIZE(szDesc), "  %12I64x %8I64x %8I64x: %3s %3s %4s %3s : %s",
                      (ULONG64)base,
                      (ULONG64)base - (ULONG64)mbi.AllocationBase,
                      (ULONG64)next - (ULONG64)base,
                      szType,
                      szState,
                      szProtect,
                      szAllocProtect,
                      pszFile);


        }
        else {
            sprintf_s(szDesc, ARRAYSIZE(szDesc), "  %12s %8I64x %8I64x: %3s %3s %4s %3s : %s",
                      "-",
                      (ULONG64)base - (ULONG64)mbi.AllocationBase,
                      (ULONG64)next - (ULONG64)base,
                      szType,
                      szState,
                      szProtect,
                      szAllocProtect,
                      pszFile);
        }
        printf("%s\n", szDesc);
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////// main.
//
int CDECL main(int argc, char **argv)
{
    BOOLEAN fNeedHelp = FALSE;
    BOOLEAN fVerbose = FALSE;
    PCHAR pszDllPath = NULL;

    int arg = 1;
    for (; arg < argc && (argv[arg][0] == '-' || argv[arg][0] == '/'); arg++) {

        CHAR *argn = argv[arg] + 1;
        CHAR *argp = argn;
        while (*argp && *argp != ':' && *argp != '=')
            argp++;
        if (*argp == ':' || *argp == '=')
            *argp++ = '\0';

        switch (argn[0]) {
          case 'd':                                     // Set DLL Name
          case 'D':
            pszDllPath = argp;
            break;

          case 'v':                                     // Verbose
          case 'V':
            fVerbose = TRUE;
            break;

          case '?':                                     // Help
            fNeedHelp = TRUE;
            break;

          default:
            fNeedHelp = TRUE;
            printf("withdll.exe: Bad argument: %s\n", argv[arg]);
            break;
        }
    }

    if (arg >= argc) {
        fNeedHelp = TRUE;
    }

    if (pszDllPath == NULL) {
        fNeedHelp = TRUE;
    }

    if (fNeedHelp) {
        PrintUsage();
        return 9001;
    }

    /////////////////////////////////////////////////////////// Validate DLLs.
    //
    CHAR szDllPath[1024];
    PCHAR pszFilePart = NULL;

    if (!GetFullPathName(pszDllPath, ARRAYSIZE(szDllPath), szDllPath, &pszFilePart)) {
        printf("withdll.exe: Error: %s is not a valid path name..\n",
               pszDllPath);
        return 9002;
    }

    HMODULE hDll = LoadLibraryEx(pszDllPath, NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (hDll == NULL) {
        printf("withdll.exe: Error: %s failed to load (error %d).\n",
               pszDllPath,
               GetLastError());
        return 9003;
    }

    ExportContext ec;
    ec.fHasOrdinal1 = FALSE;
    ec.nExports = 0;
    DetourEnumerateExports(hDll, &ec, ExportCallback);
    FreeLibrary(hDll);

    if (!ec.fHasOrdinal1) {
        printf("withdll.exe: Error: %s does not export ordinal #1.\n",
               pszDllPath);
        printf("             See help entry DetourCreateProcessWithDllEx in Detours.chm.\n");
        return 9004;
    }

    //////////////////////////////////////////////////////////////////////////
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    CHAR szCommand[2048];
    CHAR szExe[1024];
    CHAR szFullExe[1024] = "\0";
    PCHAR pszFileExe = NULL;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    szCommand[0] = L'\0';

#ifdef _CRT_INSECURE_DEPRECATE
    strcpy_s(szExe, sizeof(szExe), argv[arg]);
#else
    strcpy(szExe, argv[arg]);
#endif
    for (; arg < argc; arg++) {
        if (strchr(argv[arg], ' ') != NULL || strchr(argv[arg], '\t') != NULL) {
#ifdef _CRT_INSECURE_DEPRECATE
            strcat_s(szCommand, sizeof(szCommand), "\"");
            strcat_s(szCommand, sizeof(szCommand), argv[arg]);
            strcat_s(szCommand, sizeof(szCommand), "\"");
#else
            strcat(szCommand, "\"");
            strcat(szCommand, argv[arg]);
            strcat(szCommand, "\"");
#endif
        }
        else {
#ifdef _CRT_INSECURE_DEPRECATE
            strcat_s(szCommand, sizeof(szCommand), argv[arg]);
#else
            strcat(szCommand, argv[arg]);
#endif
        }

        if (arg + 1 < argc) {
#ifdef _CRT_INSECURE_DEPRECATE
            strcat_s(szCommand, sizeof(szCommand), " ");
#else
            strcat(szCommand, " ");
#endif
        }
    }
    printf("withdll.exe: Starting: `%s'\n", szCommand);
    printf("withdll.exe:   with `%s'\n", szDllPath);
    fflush(stdout);

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

    SetLastError(0);
    SearchPath(NULL, szExe, ".exe", ARRAYSIZE(szFullExe), szFullExe, &pszFileExe);
    if (!DetourCreateProcessWithDllEx(szFullExe[0] ? szFullExe : NULL, szCommand,
                                      NULL, NULL, TRUE, dwFlags, NULL, NULL,
                                      &si, &pi, szDllPath, NULL)) {
        DWORD dwError = GetLastError();
        printf("withdll.exe: DetourCreateProcessWithDllEx failed: %d\n", dwError);
        if (dwError == ERROR_INVALID_HANDLE) {
#if DETOURS_64BIT
            printf("withdll.exe: Can't detour a 32-bit target process from a 64-bit parent process.\n");
#else
            printf("withdll.exe: Can't detour a 64-bit target process from a 32-bit parent process.\n");
#endif
        }
        ExitProcess(9009);
    }

    if (fVerbose) {
        DumpProcess(pi.hProcess);
    }

    ResumeThread(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        printf("withdll.exe: GetExitCodeProcess failed: %d\n", GetLastError());
        return 9010;
    }

    return dwResult;
}
//
///////////////////////////////////////////////////////////////// End of File.
