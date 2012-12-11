
//////////////////////////////////////////////////////////////////////////
//
//	Filename: 	KTree.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2007-3-14  17:55
//	Comment:	普通树(数据结构)
//
//
//  树结点包含以下数据:
//  1. 	 m_data         用于存放数据
//  2.   m_pParent      指向该结点的父结点, 如果该结点为根结点, m_pParent为空
//  3.   m_pFirstChild  指向该结点的第一个子结点, 如果没有子结点, m_pFirstChild为空
//  4.   m_pNextSibling 指向该结点的下一个兄弟结点, 如果后面没有兄弟, m_pNextSibling为空
//
//  树 函数简介:
//  1.   Clear          清空树, 删除所有结点
//  2.   InsertItem     在指定的父结点的所有子结点末尾 插入一个结点
//  3.   DeleteItem     删除指定结点 以及该节点的所有子结点
//	4.   GetParent      获取指定结点的父结点, 根结点的父结点为NULL
//  5.   GetNextSibling 获取指定结点的下一个兄弟结点
//  6.   GetFirstChild  获取指定结点的第一个子结点
//  7.   HasChildren    查看指定结点是否有子结点
//  8.   Size           获取当前结点总数
//
//
//////////////////////////////////////////////////////////////////////////

#ifndef _KTREE_H_
#define _KTREE_H_
#include <assert.h>


//////////////////////////////////////////////////////////////////////////
//
// 树
//
//////////////////////////////////////////////////////////////////////////

template<class T>
class KTreeItem;



template<class T>
class KTree
{ 
public:
	KTree()
	{
		m_pRoot = NULL;
		m_nItemCount = 0;
	}

	~KTree()
	{
		Clear();
	}

	KTreeItem<T>* GetRootItem() const
	{
		return m_pRoot;
	}

	void Clear()
	{
		if( m_pRoot != NULL )
		{
			DeleteItem( m_pRoot );
		}

		assert( m_nItemCount == 0 );
	}

	KTreeItem<T>* InsertItem( const T& data, KTreeItem<T>* pParent )
	{
		KTreeItem<T>* pItem = new KTreeItem<T>;
		if( pItem == NULL )
		{
			return NULL;
		}
		m_nItemCount++;


		pItem->m_data    = data;
		pItem->m_pParent = pParent;

		if( pParent == NULL )
		{
			assert( m_pRoot == NULL );
			m_pRoot = pItem;
		}
		else
		{
			KTreeItem<T>* pIter = pParent->m_pFirstChild;

			if( pIter == NULL )
			{
				pParent->m_pFirstChild = pItem;
			}
			else
			{
				while( pIter->m_pNextSibling != NULL )
				{
					pIter = pIter->m_pNextSibling;
				}

				pIter->m_pNextSibling = pItem;
			}
		}

		return pItem;
	}


	void DeleteItem( KTreeItem<T>* pItem )
	{
		assert( pItem != NULL );

		DeleteChildren( pItem );

		if( pItem != m_pRoot )
		{
			KTreeItem<T>*  pParent = GetParent( pItem );
			KTreeItem<T>*  pIter   = pParent->m_pFirstChild;

			if( pIter == pItem )
			{
				pParent->m_pFirstChild = pIter->m_pNextSibling;
			}
			else
			{
				while( pIter->m_pNextSibling != pItem )
				{
					pIter = pIter->m_pNextSibling;
					assert( pIter != NULL );
				}

				pIter->m_pNextSibling = pItem->m_pNextSibling;
			}

		}
		else
		{
			m_pRoot = NULL;
		}

		delete pItem;
		m_nItemCount--;
	}



	KTreeItem<T>* GetParent( const KTreeItem<T>* pItem ) const
	{
		assert( pItem != NULL );
		return pItem->m_pParent;
	}

	KTreeItem<T>* GetNextSibling( const KTreeItem<T>* pItem ) const
	{
		assert( pItem != NULL );
		return pItem->m_pNextSibling;
	}


	KTreeItem<T>* GetFirstChild( const KTreeItem<T>* pItem ) const
	{
		assert( pItem != NULL );
		return pItem->m_pFirstChild;
	}


	bool HasChildren( const KTreeItem<T>* pItem ) const
	{
		return ( pItem->m_pFirstChild != NULL );
	}


	int Size() const
	{
		return m_nItemCount;
	}

private:
	void DeleteChildren( KTreeItem<T>* pItem )
	{
		KTreeItem<T>* pNext = NULL;
		KTreeItem<T>* pIter = NULL;

		for( pIter = pItem->m_pFirstChild; pIter != NULL; pIter = pNext )
		{
			pNext = pIter->m_pNextSibling;

			DeleteChildren( pIter );
			delete pIter;
			pIter = NULL;
			m_nItemCount--;
		}
	}


private: 
	KTreeItem<T>* m_pRoot;
	int           m_nItemCount;
};


//////////////////////////////////////////////////////////////////////////
//
// 树结点
//
//////////////////////////////////////////////////////////////////////////


template<class T>
class KTreeItem
{
	friend class KTree<T>;
public:
	KTreeItem()
	{
		m_pParent      = NULL;
		m_pFirstChild  = NULL;
		m_pNextSibling = NULL;
	}

	~KTreeItem()
	{
	}

	const T& GetData() const
	{
		return m_data;
	}

	T& GetData()
	{
		return m_data;
	}

	void SetData( const T& data )
	{
		m_data = data;
	}


	T      m_data;

	KTreeItem<T>*  m_pParent;  
	KTreeItem<T>*  m_pFirstChild;
	KTreeItem<T>*  m_pNextSibling;              
};



#endif // _KTREE_H_