/********************************************************************
* @file      : scoped_ptr.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/21 11:56
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _SCOPED_PTR_HPP_
#define _SCOPED_PTR_HPP_

namespace zl
{
    template<class T>
    class scoped_ptr
    {
    private:
        T* px;

        scoped_ptr(scoped_ptr const &);
        scoped_ptr& operator=(scoped_ptr const &);

        typedef scoped_ptr<T> this_type;
        void operator==( scoped_ptr const& ) const;
        void operator!=( scoped_ptr const& ) const;

    public:
        typedef T element_type;

        explicit scoped_ptr( T* p = 0 ): px( p )
        {

        }

        ~scoped_ptr()
        {
            //TODO:自定义析构
            delete px;
        }

        void reset(T * p = 0)
        {
            this_type(p).swap(*this);
        }

        T& operator*() const
        {
            return *px;
        }

        T* operator->() const
        {
            return px;
        }

        T* get() const
        {
            return px;
        }

        void swap(scoped_ptr& b)
        {
            T* tmp = b.px;
            b.px = px;
            px = tmp;
        }
    };

    template<class T> inline void swap(scoped_ptr<T>& a, scoped_ptr<T>& b)
    {
        a.swap(b);
    }

    template<class T> inline T* get_pointer(scoped_ptr<T> const& p)
    {
        return p.get();
    }

}

#endif