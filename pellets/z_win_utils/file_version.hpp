/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#pragma comment(lib, "Version.lib")

#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{

    class FileVersion
    {
    public:
        FileVersion()
        {
            Reset();
        }

        virtual ~FileVersion()
        {}

        BOOL Create(HMODULE hModule = NULL)
        {
            CString	sPath;

            GetModuleFileName(hModule, sPath.GetBuffer(_MAX_PATH), _MAX_PATH);
            sPath.ReleaseBuffer();

            return Create(sPath);
        }

        BOOL Create(LPCTSTR lpszFileName)
        {
            Reset();

            DWORD dwHandle;
            DWORD dwFileVersionInfoSize = GetFileVersionInfoSize((LPTSTR)lpszFileName, &dwHandle);

            if (!dwFileVersionInfoSize)
                return FALSE;

            LPVOID lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];
            if (lpData == NULL)
                return FALSE;

            try
            {
                if (!GetFileVersionInfo((LPTSTR)lpszFileName, dwHandle, dwFileVersionInfoSize, lpData))
                    throw FALSE;

                LPVOID lpInfo;
                UINT unInfoLen;

                if (VerQueryValue(lpData, L"\\", &lpInfo, &unInfoLen))
                {
                    if (unInfoLen == sizeof(mFileInfo))
                        memcpy(&mFileInfo, lpInfo, unInfoLen);
                }

                VerQueryValue(lpData, L"\\VarFileInfo\\Translation", &lpInfo, &unInfoLen);

                DWORD dwLangCode = 0;
                BOOL bRetCode;

                bRetCode = GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, FALSE);
                if (bRetCode == FALSE)
                {
                    bRetCode = GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, TRUE);
                    if (bRetCode == FALSE)
                    {
                        bRetCode = GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, TRUE);
                        if (bRetCode == FALSE)
                        {
                            bRetCode = GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, TRUE);
                            if (bRetCode == FALSE)
                            {
                                dwLangCode = *((DWORD*)lpInfo);
                            }
                        }
                    }
                }

                CString sSubBlock;
                CString strTemp;

                sSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000)>>16);

                strTemp = sSubBlock + L"CompanyName";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == FALSE)//¼ì²éÊÇ·ñ´æÔÚ	
                    dwLangCode = 0x04b00409;//sogou
                strTemp.ReleaseBuffer();

                sSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000)>>16);

                strTemp = sSubBlock + L"CompanyName";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msCompanyName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileDescription";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileDescription = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileVersion";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"InternalName";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msInternalName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalCopyright";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalCopyright = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"OriginalFileName";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msOriginalFileName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductName";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductVersion";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"Comments";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msComments = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalTrademarks";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalTrademarks = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"PrivateBuild";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msPrivateBuild = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"SpecialBuild";
                bRetCode = VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msSpecialBuild = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                delete[] lpData;
            }catch (BOOL)
            {
                delete[] lpData;
                return FALSE;
            }

            return TRUE;
        }

        WORD GetFileVersion(int nIndex) const
        {
            if (nIndex == 0)
                return (WORD)(mFileInfo.dwFileVersionLS & 0x0000FFFF);
            else if (nIndex == 1)
                return (WORD)((mFileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
            else if (nIndex == 2)
                return (WORD)(mFileInfo.dwFileVersionMS & 0x0000FFFF);
            else if (nIndex == 3)
                return (WORD)((mFileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
            else
                return 0;
        }

        WORD GetProductVersion(int nIndex) const
        {
            if (nIndex == 0)
                return (WORD)(mFileInfo.dwProductVersionLS & 0x0000FFFF);
            else if (nIndex == 1)
                return (WORD)((mFileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
            else if (nIndex == 2)
                return (WORD)(mFileInfo.dwProductVersionMS & 0x0000FFFF);
            else if (nIndex == 3)
                return (WORD)((mFileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
            else
                return 0;
        }

        DWORD GetFileFlagsMask() const
        {
            return mFileInfo.dwFileFlagsMask;
        }

        DWORD GetFileFlags() const
        {
            return mFileInfo.dwFileFlags;
        }

        DWORD GetFileOs() const
        {
            return mFileInfo.dwFileOS;
        }

        DWORD GetFileType() const
        {
            return mFileInfo.dwFileType;
        }

        DWORD GetFileSubtype() const
        {
            return mFileInfo.dwFileSubtype;
        }

        FILETIME GetFileDate() const
        {
            FILETIME ft;

            ft.dwLowDateTime = mFileInfo.dwFileDateLS;
            ft.dwHighDateTime = mFileInfo.dwFileDateMS;

            return ft;
        }

        CString GetCompanyName() const
        {
            return msCompanyName;
        }

        CString GetFileDescription() const
        {
            return msFileDescription;
        }

        CString GetFileVersion() const
        {
            return msFileVersion;
        }

        CString GetInternalName() const
        {
            return msInternalName;
        }

        CString GetLegalCopyright() const
        {
            return msLegalCopyright;
        }

        CString GetOriginalFileName() const
        {
            return msOriginalFileName;
        }

        CString GetProductName() const
        {
            return msProductName;
        }

        CString GetProductVersion() const
        {
            return msProductVersion;
        }

        CString GetComments() const
        {
            return msComments;
        }

        CString GetLegalTrademarks() const
        {
            return msLegalTrademarks;
        }

        CString GetPrivateBuild() const
        {
            return msPrivateBuild;
        }

        CString GetSpecialBuild() const
        {
            return msSpecialBuild;
        }

    protected:
        virtual void Reset()
        {
            ZeroMemory(&mFileInfo, sizeof(mFileInfo));

            msCompanyName.Empty();
            msFileDescription.Empty();
            msFileVersion.Empty();
            msInternalName.Empty();
            msLegalCopyright.Empty();
            msOriginalFileName.Empty();
            msProductName.Empty();
            msProductVersion.Empty();
            msComments.Empty();
            msLegalTrademarks.Empty();
            msPrivateBuild.Empty();
            msSpecialBuild.Empty();
        }

        BOOL GetTranslationId(
            LPVOID lpData, 
            UINT   unBlockSize, 
            WORD   wLangId, 
            DWORD  &dwId, 
            BOOL   bPrimaryEnough = FALSE)
        {
            LPWORD lpwData;

            for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
            {
                if (*lpwData == wLangId)
                {
                    dwId = *((DWORD*)lpwData);
                    return TRUE;
                }
            }

            if (!bPrimaryEnough)
                return FALSE;

            for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
            {
                if (((*lpwData)&0x00FF) == (wLangId&0x00FF))
                {
                    dwId = *((DWORD*)lpwData);
                    return TRUE;
                }
            }

            return FALSE;
        }

    private:
        VS_FIXEDFILEINFO mFileInfo;

        CString msCompanyName;
        CString msFileDescription;
        CString msFileVersion;
        CString msInternalName;
        CString msLegalCopyright;
        CString msOriginalFileName;
        CString msProductName;
        CString msProductVersion;
        CString msComments;
        CString msLegalTrademarks;
        CString msPrivateBuild;
        CString msSpecialBuild;
    };

}
}
