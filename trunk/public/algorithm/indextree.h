/*
 * indextree.h
 *
 *  Created on: 2012-11-26
 *      Author: kevin
 */

#ifndef INDEXTREE_HPP__
#define INDEXTREE_HPP__

#include <set>
#include "container/vector.hpp"
#include "container/string.hpp"

namespace zl
{
    class IndexTreeNode;
    struct IndexTreeNodeCompare
    {
        bool operator()(IndexTreeNode* first, IndexTreeNode* sencond) const;
    };

    class IndexTreeNode
    {
    public:
        char m_value;
        std::set<IndexTreeNode*, IndexTreeNodeCompare> m_next;
        bool m_isEnd;
        int count;
        int	id;

    public:
        IndexTreeNode()
        {
            m_value = ' ';
            m_isEnd = false;
            m_next.clear();
        }

        ~IndexTreeNode()
        {

        }

        bool compare(IndexTreeNode* second)
        {
            return this->m_value > second->m_value; 
        }
    };

    class IndexTree
    {
    private:
        IndexTreeNode*	m_root;

    protected:
        IndexTreeNode* CreateNode(char value);
        void DestoryNode(IndexTreeNode* node);
        void DestoryTree();

    public:

        IndexTree()
        {
            m_root = CreateNode(' ');
        }

        ~IndexTree()
        {
            DestoryTree();
        }

        int init(CSimpleVector<basic_string>& stringlist);

        int add(const char* Data, int len, int id);

        bool remove(const char* Data, int len);

        IndexTreeNode*	find(const char* FoundData);

        void release();
    };

}

#endif // INDEXTREE_H__