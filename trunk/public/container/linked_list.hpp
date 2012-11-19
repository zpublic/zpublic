/********************************************************************
* @file      : linked_list.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/18 14:57
* @brief     : a simple list that faster than std::list
* 
* 
*********************************************************************/

#ifndef _LINKED_LIST_HPP_
#define _LINKED_LIST_HPP_

namespace zl
{

    template <typename T>
    class LinkNode
    {
    public:
        LinkNode() : previous_(0), next_(0) {}
        LinkNode(LinkNode<T>* previous, LinkNode<T>* next)
            : previous_(previous), next_(next) {}

        void InsertBefore(LinkNode<T>* e)
        {
            this->next_ = e;
            this->previous_ = e->previous_;
            e->previous_->next_ = this;
            e->previous_ = this;
        }

        void InsertAfter(LinkNode<T>* e)
        {
            this->next_ = e->next_;
            this->previous_ = e;
            e->next_->previous_ = this;
            e->next_ = this;
        }

        void RemoveFromList()
        {
            this->previous_->next_ = this->next_;
            this->next_->previous_ = this->previous_;
        }

        LinkNode<T>* previous() const
        {
            return previous_;
        }

        LinkNode<T>* next() const
        {
            return next_;
        }

        const T* value() const
        {
            return static_cast<const T*>(this);
        }

        T* value()
        {
            return static_cast<T*>(this);
        }

    private:
        LinkNode<T>* previous_;
        LinkNode<T>* next_;
    };

    template <typename T>
    class LinkedList
    {
    public:
        LinkedList() : root_(&root_, &root_) {}

        void Append(LinkNode<T>* e)
        {
            e->InsertBefore(&root_);
        }

        LinkNode<T>* head() const
        {
            return root_.next();
        }

        LinkNode<T>* tail() const
        {
            return root_.previous();
        }

        const LinkNode<T>* end() const
        {
            return &root_;
        }

    private:
        LinkNode<T> root_;
    };
}

#endif