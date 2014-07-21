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

/**
 * @file
 * @brief 文件版本信息
 * @todo 这个文件注释,有待修改
 */


#pragma once
#pragma comment(lib, "Version.lib")

#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 提供文件版本信息相关操作
     */
    class ZLFileVersion
    {
    public:
        ZLFileVersion()
        {
            Reset();
        }

        virtual ~ZLFileVersion()
        {}

        /**
         * @brief 创建初始化指定模块文件信息
         * @param[in] hModule 模块句柄，默认为NULL
         * @return 成功返回TRUE，失败FALSE
         */
        BOOL Create(HMODULE hModule = NULL)
        {
            CString sPath;

            ::GetModuleFileName(hModule, sPath.GetBuffer(_MAX_PATH), _MAX_PATH);
            sPath.ReleaseBuffer();
            return Create(sPath);
        }

        /**
         * @brief 创建初始化指定文件信息
         * @param[in] lpFileName  文件路径
         * @return 成功返回TRUE，失败FALSE
         */
        BOOL Create(LPCTSTR lpFileName)
        {
            Reset();
            DWORD dwHandle = 0;
            DWORD dwFileVersionInfoSize = ::GetFileVersionInfoSize((LPTSTR)lpFileName, &dwHandle);

            if (!dwFileVersionInfoSize)
                return FALSE;

            LPVOID lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];
            if (lpData == NULL)
                return FALSE;

            try
            {
                if (!::GetFileVersionInfo((LPTSTR)lpFileName, dwHandle, dwFileVersionInfoSize, lpData))
                    throw FALSE;

                LPVOID lpInfo = NULL;
                UINT unInfoLen = 0;

                if (::VerQueryValue(lpData, L"\\", &lpInfo, &unInfoLen))
                {
                    if (unInfoLen == sizeof(mFileInfo))
                        memcpy(&mFileInfo, lpInfo, unInfoLen);
                }

                ::VerQueryValue(lpData, L"\\VarFileInfo\\Translation", &lpInfo, &unInfoLen);

                DWORD dwLangCode = 0;
                BOOL bRetCode = GetTranslationId(lpInfo, unInfoLen, ::GetUserDefaultLangID(), dwLangCode, FALSE);
                if (bRetCode == FALSE)
                {
                    bRetCode = GetTranslationId(lpInfo, unInfoLen, ::GetUserDefaultLangID(), dwLangCode, TRUE);
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
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == FALSE)//检查是否存在
                    dwLangCode = 0x04b00409;//sogou
                strTemp.ReleaseBuffer();

                sSubBlock.Format(L"\\StringFileInfo\\%04X%04X\\", dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000)>>16);

                strTemp = sSubBlock + L"CompanyName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msCompanyName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileDescription";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileDescription = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"FileVersion";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msFileVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"InternalName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msInternalName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalCopyright";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalCopyright = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"OriginalFileName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msOriginalFileName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductName";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductName = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"ProductVersion";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msProductVersion = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"Comments";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msComments = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"LegalTrademarks";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msLegalTrademarks = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"PrivateBuild";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
                if (bRetCode == TRUE)
                    msPrivateBuild = (LPCTSTR)lpInfo;
                strTemp.ReleaseBuffer();

                strTemp = sSubBlock + L"SpecialBuild";
                bRetCode = ::VerQueryValue(lpData, strTemp.GetBuffer(), &lpInfo, &unInfoLen);
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

        /**
         * @brief 读取文件版本
         * @param[in] nIndex 下标,从0开始,分别对应版本号中以点号间隔的各个段.
         * @return 版本号
         * @note 文件版本号由4段组成,如"2014.7.17.520", 参数nIndex填0时,取得2014, 填3时,取得520
         */
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

        /**
         * @brief 读取产品版本号
         * @param[in] nIndex 下标,从0开始, 分别对应版本号中以点号间隔的各个段.
         * @return 版本号
         * @note 文件版本号由4段组成,如"9.1.123455.888", 参数nIndex填0时,取得9, 填3时,取得888
         */
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

        /**
         * @brief 文件标志位的掩码,参考MSDN的VS_FIXEDFILEINFO结构体
         */
        DWORD GetFileFlagsMask() const
        {
            return mFileInfo.dwFileFlagsMask;
        }

        /**
         * @brief 文件标志位,参考MSDN的VS_FIXEDFILEINFO结构体
         */
        DWORD GetFileFlags() const
        {
            return mFileInfo.dwFileFlags;
        }

        /**
        * @brief 获取文件系统
        * @param[in]   nIndex  索引
        * @return FileOS
        */
        DWORD GetFileOs() const
        {
            return mFileInfo.dwFileOS;
        }

        /**
        * @brief 获取文件通用类型
        * @return FileType
        */
        DWORD GetFileType() const
        {
            return mFileInfo.dwFileType;
        }

        /**
         * @brief 获取文件功能
         * @return FileSubtype
         */
        DWORD GetFileSubtype() const
        {
            return mFileInfo.dwFileSubtype;
        }

        /**
        * @brief 获取文件日期
        * @return FILETIME data structure 
        */
        FILETIME GetFileDate() const
        {
            FILETIME ft;

            ft.dwLowDateTime = mFileInfo.dwFileDateLS;
            ft.dwHighDateTime = mFileInfo.dwFileDateMS;

            return ft;
        }

        /**
        * @brief 获取CompanyName
        * @return  字符串，代表CompanyName
        */
        CString GetCompanyName() const
        {
            return msCompanyName;
        }

        /**
        * @brief 获取文件描述
        * @return  字符串，表示文件描述
        */
        CString GetFileDescription() const
        {
            return msFileDescription;
        }

        /**
         * @brief 获取文件版本
         * @return  字符串，代表FileVersion
         */
        CString GetFileVersion() const
        {
            return msFileVersion;
        }

        /**
        * @brief 获取InternalName
        * @return  字符串，代表InternalName
        */
        CString GetInternalName() const
        {
            return msInternalName;
        }

        /**
        * @brief 获取LegalCopyright
        * @return  字符串，代表LegalCopyright
        */
        CString GetLegalCopyright() const
        {
            return msLegalCopyright;
        }

        /**
        * @brief 获取OriginalFileName
        * @return  字符串，代表OriginalFileName
        */
        CString GetOriginalFileName() const
        {
            return msOriginalFileName;
        }

        /**
         * @brief 获取ProductName
         * @return  字符串，代表ProductName
         */
        CString GetProductName() const
        {
            return msProductName;
        }

        /**
        * @brief 获取ProductVersion
        * @return  字符串，代表ProductVersion
        */
        CString GetProductVersion() const
        {
            return msProductVersion;
        }

        /**
         * @brief 获取Comments
         * @return  字符串，代表Comments
         */
        CString GetComments() const
        {
            return msComments;
        }

        /**
        * @brief 获取LegalTrademarks
        * @return  字符串，代表LegalTrademarks
        */
        CString GetLegalTrademarks() const
        {
            return msLegalTrademarks;
        }

        /**
        * @brief 获取PrivateBuild
        * @return  字符串，代表PrivateBuild
        */
        CString GetPrivateBuild() const
        {
            return msPrivateBuild;
        }

        /**
         * @brief 获取SpecialBuild
         * @return  字符串，代表SpecialBuild
         */
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

        /**
         * @brief 获取当前用户的本地语言标示
         * @param[in]   lpData Buffer，包含指定文件的版本信息
         * @param[in]   unBlockSize VS_FIXEDFILEINFO结构体的大小
         * @param[in]  wLangId 语言标示
         * @param[out]  dwId 语言标示
         * @param[in]  bPrimaryEnough 是否进行低字节操作
         * @return 如果成功，返回TRUE，如果失败返回FALSE
         */
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
