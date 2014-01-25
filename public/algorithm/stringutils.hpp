/************************************************************************
 * @file StringUtils.h
 * @author DeadMan<rookie2@163.com>
 * @ref http://www.codeproject.com/Articles/1114/STL-Split-String
 * @ref http://www.sytarena.com/cjswz/20130527/829.html
 * @date 2014-01-25
 ************************************************************************/
#ifndef __STRING_UTILS_HPP_
#define __STRING_UTILS_HPP_

#include <vector>
#include <xstring>
#include <wchar.h>


/**
 * @class StringUtils
 * @brief 实现对字符串的切割
 * @details
 *      共2个实现算法, 每个算法均有两个版本(ANSI版和UNICODE版)
 * @usage
 *      StringUtils::SplitStringA(",,1,2,,3,4,,,5", ',', vecResults); // { "1", "2", "3", "4", "5"};
 *      
 *      StringUtils::SplitStringA("1,,2,,", ",", vecResults, false); // {"1", "", "2", "", ""};
 *      StringUtils::SplitStringA(",,1,2,,3,4,,,5", ",", vecResults, true); // { "1", "2", "3", "4", "5"};
 */
class StringUtils
{
public:
    static void SplitStringA(const std::string& input,  const char splitChar,    std::vector<std::string>& output)
    {
        output.clear();
        std::string::size_type loc_start = input.find_first_not_of(splitChar);
        std::string::size_type loc_stop = loc_start;
        while(loc_stop != std::string::npos)
        {
            loc_stop = input.find_first_of(splitChar, loc_start);
            if (loc_stop != std::string::npos)
                output.push_back(input.substr(loc_start, loc_stop - loc_start));
            else
                output.push_back(input.substr(loc_start));
            loc_start = input.find_first_not_of(splitChar, loc_stop);
            loc_stop = loc_start;
        }
    }

    static void SplitStringW(const std::wstring& input, const wchar_t splitChar, std::vector<std::wstring>& output)
    {
        output.clear();
        std::wstring::size_type loc_start = input.find_first_not_of(splitChar);
        std::wstring::size_type loc_stop = loc_start;
        while(loc_stop != std::wstring::npos)
        {
            loc_stop = input.find_first_of(splitChar, loc_start);
            if (loc_stop != std::wstring::npos)
                output.push_back(input.substr(loc_start, loc_stop - loc_start));
            else
                output.push_back(input.substr(loc_start));
            loc_start = input.find_first_not_of(splitChar, loc_stop);
            loc_stop = loc_start;
        }
    }

    static int  SplitStringA(const std::string& input,  const std::string& delimiter,  std::vector<std::string>& output, bool includeEmpties)
    {
        output.clear();
        int iPos = 0;
        int newPos = -1;
        int sizeS2 = (int)delimiter.size();
        int isize = (int)input.size();

        if ( ( isize == 0 ) || ( sizeS2 == 0 ) )
            return 0;

        std::vector<int> positions;

        newPos = (int)input.find(delimiter, 0);

        if( newPos < 0 )
        { 
            return 0; 
        }

        int numFound = 0;

        while( newPos >= iPos )
        {
            numFound++;
            positions.push_back(newPos);
            iPos = newPos;
            newPos = (int)input.find(delimiter, iPos+sizeS2);
        }

        if( numFound == 0 )
            return 0;

        int offset = 0;
        for( int i=0; i<=(int)positions.size(); ++i )
        {
            std::string s("");
            if( i == 0 ) 
            { 
                s = input.substr( i, positions[i] ); 
            }
            else
            {
                offset = positions[i-1] + sizeS2;
            }
            if( offset < isize )
            {
                if( i == positions.size() )
                {
                    s = input.substr(offset);
                }
                else if( i > 0 )
                {
                    s = input.substr( positions[i-1] + sizeS2, 
                        positions[i] - positions[i-1] - sizeS2 );
                }
            }
            if( includeEmpties || ( s.size() > 0 ) )
            {
                output.push_back(s);
            }
        }
        return numFound;
    }

    static int  SplitStringW(const std::wstring& input, const std::wstring& delimiter, std::vector<std::wstring>& output, bool includeEmpties)
    {
        output.clear();
        int iPos = 0;
        int newPos = -1;
        int sizeS2 = (int)delimiter.size();
        int isize = (int)input.size();

        if ( ( isize == 0 ) || ( sizeS2 == 0 ) )
            return 0;

        std::vector<int> positions;

        newPos = (int)input.find(delimiter, 0);

        if( newPos < 0 )
        { 
            return 0; 
        }

        int numFound = 0;

        while( newPos >= iPos )
        {
            numFound++;
            positions.push_back(newPos);
            iPos = newPos;
            newPos = (int)input.find(delimiter, iPos+sizeS2);
        }

        if( numFound == 0 )
            return 0;

        int offset = 0;
        for( int i=0; i<=(int)positions.size(); ++i )
        {
            std::wstring s(L"");
            if( i == 0 ) 
            { 
                s = input.substr( i, positions[i] ); 
            }
            else
            {
                offset = positions[i-1] + sizeS2;
            }
            if( offset < isize )
            {
                if( i == positions.size() )
                {
                    s = input.substr(offset);
                }
                else if( i > 0 )
                {
                    s = input.substr( positions[i-1] + sizeS2, 
                        positions[i] - positions[i-1] - sizeS2 );
                }
            }
            if( includeEmpties || ( s.size() > 0 ) )
            {
                output.push_back(s);
            }
        }
        return numFound;
    }
};


#endif // __STRING_UTILS_HPP_
