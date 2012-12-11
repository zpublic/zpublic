/********************************************************************
* @file      : pack_zip.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/29 17:15
* @brief     : zipµÄÑ¹Ëõ½âÑ¹
* 
* 
*********************************************************************/

#ifndef _PACK_ZIP_HPP_
#define _PACK_ZIP_HPP_

#include "../../../3rdparty/xzip/XZip.h"
#include "../../../3rdparty/xzip/XUnzip.h"
#include <atlstr.h>
#include <atldbcli.h>

namespace zl
{

    class Zip
    {
    private:
        HZIP m_ZipFile;

    public:
        Zip() : m_ZipFile(NULL)
        {
        }
        Zip(LPCTSTR sPath)
        {
            this->Zip::Zip();
            Open(sPath);
        }
        Zip(BYTE* buff, DWORD size)
        {
            this->Zip::Zip();
            Open(buff, size);
        }
        virtual ~Zip()
        {
            Close();
        }

    public:
        inline BOOL IsClosed()
        {
            return (m_ZipFile == NULL);
        }
        BOOL Open(LPCTSTR sPath)
        {
            CString path(sPath);
            if (path.IsEmpty()) return FALSE;

            Close();

            WIN32_FIND_DATA fd = {0};
            HANDLE find = ::FindFirstFile(path, &fd);
            if (find == INVALID_HANDLE_VALUE)
            {
                m_ZipFile = CreateZip((void *)path.GetString(), 0, ZIP_FILENAME);
            }
            else
            {
                ::FindClose(find);
                m_ZipFile = OpenZip((void *)path.GetString(), 0, ZIP_FILENAME);
            }
            return (!IsClosed());
        }
        BOOL Open(BYTE* buff, DWORD size)
        {
            Close();
            m_ZipFile = OpenZip(buff, size, ZIP_MEMORY);
            return (!IsClosed());
        }
        void Close()
        {
            if (IsClosed()) return;
            CloseZip(m_ZipFile);
            m_ZipFile = NULL;
        }

        BOOL ZipFile(LPCTSTR sPath)
        {
            if (IsClosed()) return FALSE;
            CString path(sPath);
            if (path.IsEmpty()) return FALSE;

            WIN32_FIND_DATA fd = {0};
            HANDLE find = ::FindFirstFile(path, &fd);
            if (find == INVALID_HANDLE_VALUE) return FALSE;
            ::FindClose(find);

            TCHAR szName[MAX_PATH] = {0};
            strcpyT(szName, MAX_PATH - 1, path.GetString());
            ::PathStripPath(szName);
            return (ZR_OK == ZipAdd(m_ZipFile, szName, (void *)path.GetString(), 0, ZIP_FILENAME));
        }

        BOOL UnzipFile(LPCTSTR sPath)
        {
            if (IsClosed()) return FALSE;
            CString path(sPath);
            if (path.IsEmpty()) return FALSE;
            if (path.GetAt(path.GetLength() - 1) != _T('\\'))
                path += _T('\\');

#ifdef _UNICODE
            ZIPENTRYW ze;
#else
            ZIPENTRY ze; 
#endif
            ZRESULT zr = GetZipItem(m_ZipFile, -1, &ze);
            if (zr != ZR_OK) return FALSE;

            BOOL ret = FALSE;
            int i = 0, count = ze.index;
            for(; i < count; ++i)
            {
                ZRESULT zr = GetZipItem(m_ZipFile, i, &ze);
                if (zr != ZR_OK) break;
                zr = UnzipItem(m_ZipFile, ze.index, (void*)(LPCTSTR)(path + ze.name), 0, ZIP_FILENAME);
                if (zr != ZR_OK) break;
            }
            if (i == count) ret = TRUE;
            return ret;
        }
    };

}

#endif
