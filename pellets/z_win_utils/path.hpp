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
 * @brief �ļ�·������
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
     /**
     * @brief �ṩ��windows�����ļ�·���Ĳ���
     */
    class ZLPath
    {
    public:
        /**
         * @brief ��ȡģ��ȫ·��
         * @param[in] hModule ģ����
         * @return �ɹ�����ģ��ȫ·��, ʧ�ܷ��ؿմ�
         */
        static CString GetModuleFullPath(HMODULE hModule)
        {
            DWORD dwRet = 0;
            CString strTemp;
            TCHAR szFileName[MAX_PATH + 1] = {0};
            dwRet = ::GetModuleFileName(hModule, szFileName, MAX_PATH);
            if (dwRet)
                strTemp = szFileName;
            return strTemp;
        }
        /**
         * @brief ��ȡ�ļ���
         * @param[in] szFullPath �ļ�·��
         * @return �ɹ������ļ���, ʧ�ܷ���·��
         */
        static CString PathToFileName(LPCTSTR szFullPath)
        {
            CString strPath(szFullPath);
            strPath.Replace(_T('/'), _T('\\'));
            int nPos = strPath.ReverseFind(_T('\\'));
            if (nPos == -1)
                return strPath;
            else
                return strPath.Right(strPath.GetLength() - nPos - 1);
        }
                
        /**
         * @brief ��ȡ�ļ���·��
         * @param[in] szFullPath �ļ�ȫ·��
         * @return �ɹ������ļ���·����, ʧ�ܷ��ؿմ�
         */
        static CString PathToFolderPath(LPCTSTR szFullPath)
        {
            CString strTemp(szFullPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('\\'));
            if (-1 == nPos)
                nPos = strTemp.ReverseFind(_T('/'));
            if (-1 == nPos)
                return _T("");
            return strTemp.Left(nPos + 1);
        }
        /**
         * @brief ��ȡģ���ļ���
         * @param[in] hModule ģ����
         * @return �ɹ������ļ���·����, ʧ�ܷ��ؿմ�
         */
        static CString GetModuleFolder(HMODULE hModule)
        {
            return PathToFolderPath(GetModuleFullPath(hModule));
        }
        /**
         * @brief ȥ���ļ���׺
         * @param[in] szFileName �ļ���
         * @return �ɹ������ļ���, ʧ�ܷ��ش���׺���ļ���
         */
        static CString FileNameRemoveSuffix(LPCTSTR szFileName)
        {
            CString strTemp(szFileName);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Left(nPos);
        }
        /**
         * @brief ��ȡ·���µ��ļ���׺
         * @param[in] szFullPath ȫ·��
         * @return �ɹ������ļ���׺
         */
        static CString PathToSuffix(LPCTSTR szFullPath)
        {
            CString strTemp = PathToFileName(szFullPath);
            int nPos = -1;
            nPos = strTemp.ReverseFind(_T('.'));
            if (-1 == nPos) 
                return strTemp;
            return strTemp.Mid(nPos + 1);
        }
        /**
         * @brief ��ȡ���ļ���
         * @param[in] szFullPath ȫ·��
         * @return �ɹ����ظ��ļ�����, ʧ�ܷ���·��
         */
        static CString GetParsentFolder(LPCTSTR szFullPath)
        {
            CString strParsentPath(szFullPath);
            PathRemoveBackslash(strParsentPath);
            PathRemoveFileSpec(strParsentPath);
            if (strParsentPath.IsEmpty())
                strParsentPath = szFullPath;
            return strParsentPath;
        }
        /**
         * @brief ��ȡ��·��
         * @param[in] szFullPathȫ·��
         * @return �ɹ����ظ�·��, ʧ�ܷ���ȫ·��
         */
        static CString GetRootPath(LPCTSTR szFullPath)
        {
            CString strRoot(szFullPath);
            CString strTempPath(szFullPath);
            int nPos = strTempPath.Find(_T('\\'));
            if (nPos != -1)
                strRoot = strTempPath.Left(nPos);
            return strRoot;
        }
        /**
         * @brief ��ȡ�Ϸ�·��
         * @param[in] strPath ·��
         * @return �ɹ�����·��
         */
        static CString LegalizePath(const CString &strPath)
        {
            CString strOriginPath = strPath;
            CString strData(strOriginPath);
            CString strFolderPath;
            CString strDstPath;
            while (1)
            {
                strFolderPath = GetRootPath(strData);
                if (strFolderPath == _T(".."))
                    strDstPath = GetParsentFolder(strDstPath);
                else
                    strDstPath.Append(strFolderPath);
                PathAddBackslash(strDstPath);
                strData = strData.Mid(strFolderPath.GetLength());
                strData.TrimLeft(L"\\");
                if (strData.IsEmpty())
                    break;
            }
            return strDstPath;
        }
        /**
         * @brief ·���ӷ�б��
         * @param[in] strPath ·��
         * @return ����·��
         */
        static void PathAddBackslash(CString& strPath)
        {
            CString strTemp;
            strTemp = strPath.Right(1);
            if (strTemp != _T("\\") && strTemp != _T("/"))
                strPath.Append(_T("\\"));
        }
        /**
         * @brief ȥ��·����б��
         * @param[in] strPath ·��
         * @return ����ȥ����б�ߺ��·��
         */
        static void PathRemoveBackslash(CString &strPath)
        {
            if (strPath.Right(1) == _T("\\"))
                strPath = strPath.Left(strPath.GetLength() - 1);
        }

        static void PathRemoveFileSpec(CString& strPath)
        {
            int nPos = strPath.ReverseFind(_T('\\'));
            if (nPos == -1)
                strPath.Empty();
            else
			{
                strPath = strPath.Left(nPos+1);
			}
        }
    };

}
}
