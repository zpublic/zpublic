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
 * @brief 自身实现的CString
 */


#pragma once

#include <algorithm>
#include <string>
#include <cctype>

#include <stdarg.h>
#include <assert.h>
#ifndef _DEBUG
#define ASSERT(expr)
#else
#define ASSERT(expr) assert(expr)
#endif

class CStringW : public std::wstring
{
public:
    void Append( const wchar_t * pstr )
    {
        this->append(pstr);
        this->TrimRight(L'\0');
    }
    void Append( const CStringW& string1 )
    {
        this->append(string1, string1.GetLength());
    }
    void Append( const CStringW& string1, int nLen )
    {
        this->append(string1.Left(nLen));
    }
    void Append( const wchar_t * pstr, int nLen )
    {
        this->append(pstr, nLen);
        this->TrimRight(L'\0');
    }
    void AppendChar( const wchar_t sz )
    {
        this->append(&sz);
    }
    int Delete( int nIndex, int nCount = 1 )
    {
        this->erase(nIndex, nCount);
        return this->GetLength();
    }
    int Insert( int nIndex, const wchar_t * pstr )
    {
        this->insert(nIndex, pstr);
        return this->GetLength();
    }
    int Insert( int nIndex, wchar_t ch )
    {
        CStringW strTmp(ch);
        this->insert(nIndex, strTmp);
        strTmp.Empty();
        return this->GetLength();
    }
    int Remove( wchar_t ch )
    {
        CStringW::iterator iter;
        int count = 0;
        for(iter = this->begin(); iter != this->end(); ++iter)
        {
            if(*iter == ch)
            {
                this->erase(iter);
                count++;
            }
        }
        return count;
    }
    void MakeReverse()
    {
        CStringW strTmp;
        CStringW::iterator iter;
        iter = this->end();
        iter--;
        for(; iter != this->begin(); iter--)
        {
            strTmp += *iter;
        }
        strTmp += *iter;
        *this = strTmp;
        strTmp.Empty();
    }
    int Find( wchar_t ch ) const
    {
        return this->find(ch);
    }
    int Find( const wchar_t * lpszSub ) const
    {
        return this->find(lpszSub);
    }
    int Find( wchar_t ch, int nStart ) const
    {
        return this->find(ch,nStart);
    }
    int Find( const wchar_t * pstr, int nStart ) const
    {
        return this->find(pstr,nStart);
    }
    int ReverseFind( wchar_t ch ) const
    {
        return this->find_last_of(ch);
    }
    int FindOneOf( const wchar_t * lpszCharSet ) const
    {
        return this->find_first_of(lpszCharSet);
    }
    int Format(const wchar_t* pstrFormat, ... )
    {///本函数仅仅支持ANSI标准字符集 '%[flags] [width] [.precision] [{h | l | I64 | L}]type'
        ASSERT(pstrFormat!=NULL);
        this->Empty();

        va_list argList;
        va_start(argList,pstrFormat);
        int nMaxLen = 0;
        for (const wchar_t * p = pstrFormat; *p != L'\0';p++ )
        {
            if (*p != L'%' || *(++p) == L'%')
            {// 如果不是'%'就直接累计长度，如果是'%%'也使长度加1
                nMaxLen += 1;
                continue;
            }
            int nItemLen = 0; //用来保存每个参数的长度
            int nWidth = 0; //用来保存每一项的宽度
            for (; *p != L'\0'; p ++)
            {
                if (*p == L'#')
                    nMaxLen += 2;   // 处理 '0x'
                else if (*p == L'*')
                    nWidth = va_arg(argList, int);  //如：'%5f' 中的5
                else if (*p == L'-' || *p == L'+' || *p == L'0'|| *p == L' ')
                    continue;
                else // 不是标志字符就退出循环
                    break;
            }
            if (nWidth == 0)
            { //提取宽度
                nWidth = _wtoi(p);
                for (; *p != L'\0' && isdigit(*p); p ++)
                    ;
            }
            ASSERT(nWidth >= 0);//有效宽度
            int nPrecision = 0; //精度位数
            if (*p == L'.')
            {
                p++;// 跳过 '.'字符 (宽度.精度)
                if (*p == L'*')
                { //有参数给出
                    nPrecision = va_arg(argList, int);
                    p ++;// 取得精度，跳过字符
                }
                else
                { //在格式串中有宽度
                    nPrecision = _wtoi(p);
                    for (; *p != L'\0' && isdigit(*p);	p ++)
                        ;
                }
                ASSERT(nPrecision >= 0);//有效宽度
            }
            switch (*p)
            {
            case L'h':     //short int 型
                p ++;
                break;
            case L'l':	 //long double 型
                p ++;
                break;
            case L'F':	 //近指针
            case L'N':	//远指针
            case L'L':	//long double 型
                p++;
                break;
            }
            switch (*p)
            {
            case L'c':   //// 单个字符
            case L'C':
                nItemLen = 2;
                va_arg(argList, int);
                break;
            case L's':	//// 字符串
            case L'S':
                nItemLen = wcslen(va_arg(argList, const wchar_t*));
                nItemLen = ((1) > (nItemLen)) ? (1) : (nItemLen);//如果是空串就使用1 即保存'\0'
                break;
            }
            if (nItemLen != 0)
            {
                nItemLen = ((nItemLen) > (nWidth)) ? (nItemLen) : (nWidth);//使用大者
                if (nPrecision != 0)
                    nItemLen = ((nItemLen) < (nPrecision)) ? (nItemLen) : (nPrecision);
            }
            else
            {
                switch (*p)
                {
                case L'd':    //整数的处理
                case L'i':
                case L'u':
                case L'x':
                case L'X':
                case L'o':
                    va_arg(argList, int);
                    nItemLen = 32;  //四字节
                    nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者
                    break;
                case L'e':	//浮点数
                case L'f':
                case L'g':
                case L'G':
                    va_arg(argList, double);
                    nItemLen = 32;//四字节
                    nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
                    break;
                case L'p':	//指针
                    va_arg(argList, void*);
                    nItemLen = 32;
                    nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
                    break;
                case L'n':
                    va_arg(argList, int*); //指向整数的指针,见BorlanderC++3.1库函数P352
                    break;
                default:
                    ASSERT(false);  //不能处理的格式，给出警告
                    break;
                }
            }
            nMaxLen += nItemLen;//把该项的长度累计
        }
        va_end(argList);
        va_start(argList, pstrFormat);  // 重新开始提取参数
        wchar_t* ch = new wchar_t[nMaxLen+1]; //分配内存
        vswprintf_s(ch, nMaxLen, pstrFormat, argList);
        this->append(ch); //加到string的尾部
        delete[] ch; //释放内存
        va_end(argList);
        return nMaxLen;
    }
    int GetLength() const
    {
        return this->length();
    }
    CStringW Left(int nCount) const
    {
        if (nCount <=0)
            return CStringW(L"");
        CStringW strTmp;
        strTmp = this->substr(0,nCount);
        return strTmp;
    }
    CStringW Right(int nCount) const
    {
        if (nCount <=0)
            return CStringW(L"");
        CStringW strTmp;
        if (nCount > GetLength())
            strTmp = this->substr(0);
        else
            strTmp = this->substr(GetLength() - nCount);
        return strTmp;
    }
    CStringW Mid(int nFirst) const
    {
        CStringW strTmp;
        if (nFirst >= GetLength())
            return CStringW(L"");
        if (nFirst <= 0)
            strTmp = this->substr(0);
        else
            strTmp = this->substr(nFirst);
        return strTmp;
    }
    CStringW Mid( int nFirst, int nCount) const
    {
        if (nCount <= 0)
            return CStringW(L"");
        if (nFirst >= GetLength())
            return CStringW(L"");
        CStringW strTmp;
        if (nFirst <= 0)
            strTmp = this->substr(0, nCount);
        else
            strTmp = this->substr(nFirst, nCount);
        return strTmp;
    }
    CStringW& operator=(const std::wstring str)
    {
        if (this->compare(str) == 0)
            return *this;
        this->assign(str);
        return *this;
    }
    CStringW& operator=(wchar_t ch)
    {
        this->Empty();
        this->insert(this->begin(),ch);
        return *this;
    }
    CStringW& operator =( const wchar_t * lpsz )
    {
        this->Empty();
        this->append(lpsz);
        this->TrimRight(L'\0');
        return *this;
    }
    bool operator==(const CStringW& string1) const
    {
        if (this->compare(string1) == 0)
            return true;
        return false;
    }
    bool operator==(const wchar_t * lpsz) const
    {
        if (this->compare(lpsz) == 0)
            return true;
        return false;
    }
    void MakeUpper()
    {
        std::transform(this->begin (),
            this->end (),this->begin (),
            ::toupper);
    }
    void MakeLower()
    {
        std::transform(this->begin (),
            this->end (),this->begin (),
            ::tolower);
    }
    bool IsEmpty( ) const
    {
        return this->empty();
    }
    void Empty( )
    {
        this->erase(this->begin(),this->end());
        if (strBuffer != NULL)
            delete []strBuffer;
    }
    wchar_t GetAt( int nIndex ) const
    {
        return this->at(nIndex);
    }
    wchar_t operator []( int nIndex ) const
    {
        return this->at(nIndex);
    }
    void SetAt( int nIndex, wchar_t ch )
    {
        this->at(nIndex) = ch;
    }
    operator const wchar_t * ( ) const
    {
        return this->c_str();
    }
    friend CStringW operator + ( const CStringW& string1, wchar_t* ch )
    {
        CStringW str;
        str.append(string1);
        str.append(ch);
        return str;
    }
    friend CStringW operator + (const CStringW& string1, const CStringW& string2)
    {
        CStringW str;
        str.append(string1);
        str.append(string2);
        return str;
    }
    friend CStringW operator + ( const CStringW& string1, wchar_t ch )
    {
        CStringW str;
        str.append(string1);
        str.insert(str.end(),ch);
        return str;
    }
    int Compare( const wchar_t * lpsz ) const
    {
        CStringW str;
        str.append(lpsz);
        return this->compare(str);
    }
    int Compare( const CStringW& string1 ) const
    {
        return this->compare(string1);
    }
    int CompareNoCase( const wchar_t * lpsz ) const
    {
        CStringW str,strThis;
        str.append(lpsz);
        strThis = (*this);
        str.MakeLower();
        strThis.MakeLower();
        return strThis.compare(str);
    }
    int CompareNoCase( const CStringW& string1 ) const
    {
        CStringW str,strThis;
        str = string1;
        strThis = (*this);
        str.MakeLower();
        strThis.MakeLower();
        return strThis.compare(str);
    }
    void Trim()
    {
        TrimLeft();
        TrimRight();
    }
    void TrimRight( )
    {
        TrimRight (L' ');
    }
    void TrimLeft( )
    {
        TrimLeft(L' ');
    }
    void Trim( wchar_t chTarget )
    {
        TrimLeft( chTarget );
        TrimRight( chTarget );
    }
    void TrimLeft( wchar_t chTarget )
    {
        std::wstring::size_type pos;
        pos = this->find_first_not_of(chTarget);
        if (pos == 0) return;
        this->erase(this->begin(), this->begin() + pos);
    }
    void TrimRight( wchar_t chTarget )
    {
        std::wstring::size_type pos;
        pos = this->find_last_not_of(chTarget);
        ++pos;
        if (pos == this->GetLength())
            return;
        this->erase(this->begin() + pos, this->end());
    }
    void Replace( wchar_t chOld, wchar_t chNew )
    {
        for(int i=0; i<this->GetLength(); i++)
        {
            if (this->at(i) == chOld)
                this->at(i) = chNew;
        }
    }
    void Replace(const wchar_t* chOld,const wchar_t* chNew )
    {
        int index = this->find(chOld);
        while (index > -1)
        {
            this->erase(index, wcslen(chOld));
            this->insert(index, chNew);
            index = this->find(chOld);
        }
    }
    wchar_t * GetBuffer( int nMinBufLength )
    {
        const wchar_t* strTmp = this->c_str();
        int nCount = wcslen(strTmp);
        if (nMinBufLength == 0)
            nMinBufLength = nCount;

        if (nCount > nMinBufLength)
            return NULL;
        if (strBuffer != NULL)
            delete []strBuffer;

        strBuffer = new wchar_t[nMinBufLength + 1];
        
        wcscpy(strBuffer, strTmp);
        strBuffer[nMinBufLength] = L'\0';
        return strBuffer;
    }
    wchar_t * GetBuffer( )
    {
        return GetBuffer(0);
    }
    void ReleaseBuffer( int nNewLength = -1 )
    {
        if (strBuffer != NULL)
        {
            this->Empty();
            this->Append(strBuffer);
            delete []strBuffer;
        }
        this->TrimRight(L'\0');
    }
    CStringW(const CStringW& string1)
    {
        strBuffer = NULL;
        this->append(string1);
    }
    CStringW(const wchar_t *ch)
    {
        strBuffer = NULL;
        if(ch != NULL)
        {
            this->Append(ch);
        }
    }
    CStringW(const wchar_t ch)
    {
        strBuffer = NULL;
        *this += ch;
    }
    CStringW()
    {
        strBuffer = NULL;
    }
    CStringW(const wchar_t *ch, int len)
    {
        strBuffer = NULL;
        this->Append(ch, len);
    }
    ~CStringW()
    {
        if (strBuffer != NULL)
            delete []strBuffer;
    }

private:
    wchar_t *strBuffer;
};

#ifdef UNICODE
typedef CStringW CString;
#endif