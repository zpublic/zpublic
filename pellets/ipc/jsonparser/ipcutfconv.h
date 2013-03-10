#ifndef ZPUBLIC_JSON_PARSER_IPC_UTFCONW
#define ZPUBLIC_JSON_PARSER_IPC_UTFCONW

#include <assert.h>
#include <Windows.h>

namespace zl
{
namespace Ipc
{

// UTF8 To UTF16
template<int t_nBufferLength = 128>
class KUTF8ToUTF16Ex
{
public:
    KUTF8ToUTF16Ex(const char* psz)
        : m_psz(m_szBuffer)
    {
        Init(psz);
    }

    virtual ~KUTF8ToUTF16Ex() throw()
    {
        if( m_psz != m_szBuffer )
        {
            delete[] m_psz;
        }
    }

    operator const wchar_t*() const
    {
        return( m_psz );
    }

    // 包含终结符'\0'在内的字符数目
    size_t CharCount()
    {
        if (m_psz == NULL)
            return 0;

        return m_nCharCount;
    }

protected:
    void Init(const char* psz)
    {
        if (psz == NULL)
        {
            m_psz = NULL;
            return;
        }

        m_nCharCount = MultiByteToWideChar(CP_UTF8, 0, psz, -1, NULL, 0);
        assert(m_nCharCount > 0);

        if(m_nCharCount > t_nBufferLength)
        {
            m_psz = new wchar_t[m_nCharCount];
            if( m_psz == NULL )
            {
                throw E_OUTOFMEMORY;
            }
        }

        m_nCharCount = MultiByteToWideChar(CP_UTF8, 0, psz, -1, m_psz, static_cast<int>(m_nCharCount));
        assert(m_nCharCount > 0);
    }

    wchar_t*   m_psz;
    wchar_t    m_szBuffer[t_nBufferLength];
    size_t     m_nCharCount;

    KUTF8ToUTF16Ex(const KUTF8ToUTF16Ex<t_nBufferLength>&) throw();
    KUTF8ToUTF16Ex<t_nBufferLength>& operator=( const KUTF8ToUTF16Ex<t_nBufferLength>& ) throw();
};

// UTF16转换到UTF8
template<int t_nBufferLength = 128>
class KUTF16ToUTF8Ex
{
public:
    KUTF16ToUTF8Ex(const wchar_t* psz)
        : m_psz(m_szBuffer)
    {
        Init(psz);
    }

    virtual ~KUTF16ToUTF8Ex() throw()
    {
        if( m_psz != m_szBuffer )
        {
            delete[] m_psz;
        }
    }

    operator const char*() const
    {
        return( m_psz );
    }

    // 包含终结符'\0'在内的字符数目
    // 对于UTF8,是指的字节长度，即ASCII长度
    size_t CharCount()
    {
        if (m_psz == NULL)
            return 0;

        return m_nCharCount;
    }

protected:
    void Init(const wchar_t* psz)
    {
        if (psz == NULL)
        {
            m_psz = NULL;
            return;
        }

        m_nCharCount = ::WideCharToMultiByte(CP_UTF8, 0, psz, -1, NULL, 0, NULL, NULL);
        assert(m_nCharCount > 0);

        if(m_nCharCount > t_nBufferLength)
        {
            m_psz = new char[m_nCharCount];
            if( m_psz == NULL )
            {
                throw E_OUTOFMEMORY;
            }
        }

        m_nCharCount = WideCharToMultiByte( CP_UTF8, 0, psz, -1, m_psz, static_cast<int>(m_nCharCount), NULL, NULL);
        assert(m_nCharCount > 0);
    }

    char*   m_psz;
    char    m_szBuffer[t_nBufferLength];
    size_t  m_nCharCount;

    KUTF16ToUTF8Ex( const KUTF16ToUTF8Ex<t_nBufferLength>& ) throw();
    KUTF16ToUTF8Ex<t_nBufferLength>& operator=( const KUTF16ToUTF8Ex<t_nBufferLength>& ) throw();
};

// UTF8转换到ANSI
template<int t_nBufferLength = 128>
class KUTF8ToANSIEx : public KUTF8ToUTF16Ex<t_nBufferLength>
{
public:
    KUTF8ToANSIEx(const char* psz)
        : KUTF8ToUTF16Ex<t_nBufferLength>(psz)
        , m_pszAnsi(m_szBufferAnsi)
    {
        Init(psz);
    }

    virtual ~KUTF8ToANSIEx() throw()
    {
        if( m_pszAnsi != m_szBufferAnsi )
        {
            delete[] m_pszAnsi;
        }
    }

    operator const char*() const
    {
        return( m_pszAnsi );
    }

    size_t CharCount()
    {
        if (m_pszAnsi == NULL)
            return 0;

        return m_nCharCountAnsi;
    }

protected:
    void Init(const char* psz)
    {
        if (psz == NULL)
        {
            m_pszAnsi = NULL;
            return;
        }

        m_nCharCountAnsi = WideCharToMultiByte(CP_ACP, 0, m_psz, -1, NULL, 0, NULL, NULL);
        assert(m_nCharCountAnsi > 0);

        if(m_nCharCountAnsi > t_nBufferLength)
        {
            m_pszAnsi = new char[m_nCharCountAnsi];
            if( m_pszAnsi == NULL )
            {
                throw E_OUTOFMEMORY;
            }
        }

        m_nCharCountAnsi = WideCharToMultiByte(CP_ACP, 0, m_psz, -1, m_pszAnsi, static_cast<int>(m_nCharCountAnsi), NULL, NULL);
        assert(m_nCharCountAnsi > 0);
    }

    char*   m_pszAnsi;
    char    m_szBufferAnsi[t_nBufferLength];
    size_t  m_nCharCountAnsi;

    KUTF8ToANSIEx( const KUTF8ToANSIEx<t_nBufferLength>& ) throw();
    KUTF8ToANSIEx<t_nBufferLength>& operator=( const KUTF8ToANSIEx<t_nBufferLength>& ) throw();
};

#define KUTF8ToUTF16(a) (LPCWSTR)KUTF8ToUTF16Ex<>(a)
#define KUTF16ToUTF8(a) (LPCSTR)KUTF16ToUTF8Ex<>(a)
#define KUTF8ToANSI(a)  (LPCSTR)KUTF8ToANSIEx<>(a)

#ifdef _UNICODE
#define TCharToUTF16(a) (LPCWSTR)a
#define TCharToUTF8(a)  KUTF16ToUTF8(a)
#define UTF8ToTChar(a)  KUTF8ToUTF16(a)
#else
#define TCharToUTF16(a) KUTF8ToUTF16(a)
#define TCharToUTF8(a)  (LPCSTR)a
#define UTF8ToTChar(a)  KUTF8ToANSI(a)
#endif

}
}

#endif