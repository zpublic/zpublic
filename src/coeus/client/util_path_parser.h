/* -------------------------------------------------------------------------
//  File        :   util_path_parser.h
//  Author      :   Sum
//  Datet       :   2014/1/11 14:39
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __UTIL_PATH_PARSER_H_
#define __UTIL_PATH_PARSER_H_

#include <vector>

template <typename T>
class UtilPathParser
{
public:
    ~UtilPathParser();

    size_t Size(void);

    const T* Get(size_t n);

    BOOL Parse(const T* szPath);

protected:
    void Clear(void);

private:
    std::vector<T*> m_vecFolders;
};

template <typename T>
UtilPathParser<T>::~UtilPathParser()
{
    this->Clear();
}

template <typename T>
size_t UtilPathParser<T>::Size(void)
{
    return m_vecFolders.size();
}

template <typename T>
const T* UtilPathParser<T>::Get(size_t nIndex)
{
    if (nIndex < m_vecFolders.size())
    {
        return m_vecFolders[nIndex];
    }

    return NULL;
}


template <typename T>
BOOL UtilPathParser<T>::Parse(const T* lpszPath)
{
    BOOL bReturn = FALSE;
    UINT nIndex = 0;
    UINT nIndex_t = 0;
    T tSeperate = '\\';
    T tSeperateSecond = '/';
    T* pTempBuf = NULL;
    UINT nSize = 0;

    this->Clear();

    if (!lpszPath)
    {
        return FALSE;
    }

    for (nIndex = 0; lpszPath[nIndex] != 0; nIndex++)
    {
        for (nIndex_t = 0;
            lpszPath[nIndex + nIndex_t] != tSeperate
            && lpszPath[nIndex + nIndex_t] != tSeperateSecond
            && lpszPath[nIndex + nIndex_t] != 0;
        ++nIndex_t)
        {
        }

        if (nIndex_t == 0) goto Exit;

        nSize = (nIndex_t + 1) * sizeof(T);
        pTempBuf = new T[nSize];

        if (!pTempBuf)
        {
            goto Exit;
        }

        ::memcpy((void*)pTempBuf, (void*)&lpszPath[nIndex], nSize);
        pTempBuf[nIndex_t] = 0;
        m_vecFolders.push_back(pTempBuf);

        if (lpszPath[nIndex + nIndex_t] == 0)
        {
            break;
        }
        else
        {
            nIndex += nIndex_t;
        }
    }

    bReturn = TRUE;

Exit:
    return bReturn;
}

template <typename T>
void UtilPathParser<T>::Clear(void)
{
    T* pTemp = NULL;
    std::vector<T*>::iterator iter;

    for (iter = m_vecFolders.begin();
        iter != m_vecFolders.end();
        ++iter)
    {
        pTemp = *iter;

        if (pTemp)
        {
            delete pTemp;
        }
    }

    m_vecFolders.clear();
}

typedef UtilPathParser<char> UtilPathParserA;
typedef UtilPathParser<wchar_t> UtilPathParserW;

#endif // __UTIL_PATH_PARSER_H_