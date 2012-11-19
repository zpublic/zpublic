/********************************************************************
* @file      : ref_counted.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/19 11:12
* @brief     : 
* 
* 
*********************************************************************/

#ifndef _REF_COUNTED_HPP_
#define _REF_COUNTED_HPP_

#include <WinBase.h>
#include "../../basic/nocopyable.hpp"

#define FREIEND_REFCOUNTED(TypeName)         \
    friend class ::zl::RefCounted<TypeName>; \
    ~TypeName() {};

namespace zl
{
    class RefCountedBase
    {
    public:
        bool HasOneRef() const
        {
            return ref_count_ == 1;
        }

    protected:
        RefCountedBase()
            : ref_count_(0)
        {

        }
        ~RefCountedBase()
        {

        }

        void AddRef() const
        {
            ++ref_count_;
        }

        bool Release() const
        {
            return (--ref_count_ == 0);
        }

    private:
        mutable int ref_count_;

        DISALLOW_COPY_AND_ASSIGN(RefCountedBase);
    };

    class RefCountedThreadSafeBase
    {
    public:
        bool HasOneRef() const;

    protected:
        RefCountedThreadSafeBase()
            : ref_count_(0)
        {

        }
        ~RefCountedThreadSafeBase()
        {

        }

        void AddRef() const
        {
            ::InterlockedIncrement(reinterpret_cast<volatile unsigned long*>(&ref_count_));
        }

        bool Release() const
        {
            return (::InterlockedDecrement(reinterpret_cast<volatile unsigned long*>(&ref_count_)) == 0);
        }

    private:
        mutable unsigned long ref_count_;

        DISALLOW_COPY_AND_ASSIGN(RefCountedThreadSafeBase);
    };

    template <class T>
    class RefCounted : public RefCountedBase
    {
    public:
        RefCounted() {}

        void AddRef() const
        {
            RefCountedBase::AddRef();
        }

        void Release() const
        {
            if (RefCountedBase::Release())
            {
                delete static_cast<const T*>(this);
            }
        }

    protected:
        ~RefCounted() {}

    private:
        DISALLOW_COPY_AND_ASSIGN(RefCounted<T>);
    };

}

#endif