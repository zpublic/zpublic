/********************************************************************
* @file      : assert.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/08 9:24
* @brief     : µ÷ÊÔ¶ÏÑÔ
* 
* 
*********************************************************************/

#ifndef _ASSERT_HPP_
#define _ASSERT_HPP_

namespace zl
{

    class Error
    {
    private:
        wchar_t* description;
    public:
        Error(wchar_t* _description)
        {
            description = _description;
        }

        wchar_t* Description() const
        {
            return description;
        }
    };

#ifdef _DEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)
#endif
#ifdef NDEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION)
#endif

}

#endif
