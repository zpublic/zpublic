/********************************************************************
* @file      : scoped_arr.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/21 17:12
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _SCOPED_ARR_HPP_
#define _SCOPED_ARR_HPP_

namespace zl
{

    template<class T>
    class scoped_arr
    {
    private:
        T* px;

        scoped_arr(scoped_arr const &);
        scoped_arr & operator=(scoped_arr const &);

        typedef scoped_arr<T> this_type;
        void operator==( scoped_arr const& ) const;
        void operator!=( scoped_arr const& ) const;

    public:
        typedef T element_type;

        explicit scoped_arr( T* p = 0 ) : px( p )
        {
            //TODO:自定义构造
        }

        ~scoped_arr()
        {
            //TODO:自定义析构
            delete[] px;
        }

        void reset(T* p = 0)
        {
            this_type(p).swap(*this);
        }

        T& operator[](size_t i) const
        {
            return px[i];
        }

        T* get() const
        {
            return px;
        }

        void swap(scoped_arr& b)
        {
            T* tmp = b.px;
            b.px = px;
            px = tmp;
        }
    };

    template<class T>
    inline void swap(scoped_arr<T>& a, scoped_arr<T>& b)
    {
        a.swap(b);
    }

} // namespace boost}

#endif // !_SCOPED_ARR_HPP_
