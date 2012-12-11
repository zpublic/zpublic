#include "indextree.h"
#include <queue>

namespace zl
{
    bool IndexTreeNodeCompare::operator()(IndexTreeNode* first, IndexTreeNode* sencond) const
    {
        if(first == sencond)
            return true;

        return first->compare(sencond);
    }

    IndexTreeNode* IndexTree::CreateNode(char value)
    {
        IndexTreeNode* node = new IndexTreeNode;
        node->m_value = value;
        node->m_next.clear();
        node->m_isEnd = false;
        node->count = 1;

        return node;
    }

    void IndexTree::DestoryNode(IndexTreeNode* node)
    {
        delete node;
    }

    void IndexTree::DestoryTree()
    {
        release();
        delete m_root;
    }


    int IndexTree::init(CSimpleVector<basic_string>& stringlist)
    {
        if(m_root == NULL)
        {
            m_root = CreateNode(' ');
        }
        for(int i = 0; i < stringlist.GetSize(); i++)
        {
            if(!add(stringlist[i].c_str(), stringlist[i].Size(),i))
                return false;
        }
        return true;
    }

    int IndexTree::add(const char* Data, int len, int id)
    {
        IndexTreeNode* cur = m_root;
        std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator iter;
        std::pair<std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator, bool> ret;

        for(int i=0; i<len; i++)
        {
            IndexTreeNode* node = CreateNode(Data[i]);
            iter = cur->m_next.find(node);
            if(iter == cur->m_next.end())
            {
                ret = cur->m_next.insert(node);
                if(ret.second == true)
                {
                    cur = *ret.first;
                    if(i == len-1)
                    {
                        cur->m_isEnd = true;
                        cur->id = id;
                    }
                }
                else
                    return false;
            }
            else
            {
                (*iter)->count++;
                cur = *iter;
                DestoryNode(node);
            }
        }

        return true;
    }

    bool IndexTree::remove(const char* Data, int len)
    {
        IndexTreeNode* cur = m_root;
        std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator iter;
        std::pair<std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator, bool> ret;
        zl::CSimpleVector<IndexTreeNode*> vecNode;
        bool flag = true;

        for(int i=0; i<len; i++)
        {
            IndexTreeNode* node = CreateNode(Data[i]);
            iter = cur->m_next.find(node);
            DestoryNode(node);
            if(iter != cur->m_next.end())
            {
                vecNode.Add(*iter);
                cur = *iter;
            }
            else
            {
                flag = false;
                break;
            }
        }

        if(flag)
        {
            for(int i = 0; i < vecNode.GetSize(); i++)
            {
                if(vecNode[i]->count == 0)
                {
                    DestoryNode(vecNode[i]);
                }
                else
                {
                    vecNode[i]->count--;
                }
            }
        }
        return false;
    }


    IndexTreeNode*	IndexTree::find(const char* FoundData)
    {
        int nLen = strlen(FoundData);
        IndexTreeNode* cur = m_root;
        std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator iter;

        for(int i = 0; i < nLen; i++)
        {
            IndexTreeNode* node = CreateNode(FoundData[i]);
            iter = cur->m_next.find(node);
            DestoryNode(node);
            if(iter == cur->m_next.end())
            {
                return NULL;
            }
            else
            {
                cur = *iter;
            }
        }

        if(cur->m_isEnd == true)
            return cur;	
        else
            return NULL;
    }

    void IndexTree::release()
    {
        std::queue<IndexTreeNode*> q;
        std::set<IndexTreeNode*, IndexTreeNodeCompare>::iterator iter;

        q.push(m_root);
        while(!q.empty())
        {
            IndexTreeNode* temp = q.front();
            if(!temp->m_next.empty())
            {
                for(iter = temp->m_next.begin(); iter != temp->m_next.end(); iter++)
                {
                    q.push(*iter);
                }
            }
            q.pop();
            if(temp != m_root)
                delete(temp);
        }
        m_root->m_next.clear();
    }

}