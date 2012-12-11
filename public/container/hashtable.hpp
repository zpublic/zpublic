/*
 * hashtable.hpp
 *
 *  Created on: 2012-11-20
 *      Author: kevin
 *		email:zhangtaopy-kevin@hotmail.com
 */
#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__

//#include <memory.h>

namespace zl
{
	// -------------------------------------------------------------------------
	static const int number_of_primes = 28;
	static const unsigned long prime_list[number_of_primes] = 
	{
		53,				97,				193,			389,			769,
		1543,			3079,			6151,			12289,			24593,
		49157,			98317,			196613,			393241,			786433,
		1572869,		3145739,		6291469,		12582917,		25165843,
		50331653,		100663319,		201326611,		402653189,		805306457,
		1610612741,		3221225473ul,	4294967291ul	
	};

	template <class Value>
	struct __hash_node
	{
		__hash_node* next;
		Value val;
	};

	template <class Value, class Hashfunc, class EqualKey>
	class HashTable
	{
	public:
		typedef Hashfunc hasher;
		typedef EqualKey key_equal;
		typedef __hash_node<Value> node;

	public:
		~HashTable();
		HashTable(unsigned long ulSize);
		HashTable(unsigned long ulSize, const Hashfunc& hf, const EqualKey& eq);
		int	insert_unique(const Value& obj);
		int	insert_equal(const Value& obj);
		const Value* find(const Value& obj);

	protected:
		int  insert_unique_noresize(const Value& obj);
		int	 insert_equal_noresize(const Value& obj);
		void init_bucket(unsigned long ulSize);
		void resize(bool isUnique);

	private:
		unsigned long	m_ulFullSize;
		unsigned long	m_ulCurSize;
		hasher			m_hash;
		key_equal		m_equals;
		node**			m_bucket;
	};


	static unsigned long LowerBound(const unsigned long* primelist, int n, unsigned long key)
	{
		int start = 0;
		int end = n-1;

		if(n == 0)
			return -1;

		while(start <= end)
		{
			int mid = (end - start)/2 + start;
			if(key > primelist[mid])
			{
				start = mid + 1;
			}
			else if(key < primelist[mid])
			{
				end = mid - 1;
			}
			else
			{
				return primelist[mid];
			}
		}
		if(end != n-1)
			return primelist[end+1];
		else
			return primelist[n];
	}


	template <class V, class Hf, class Eq>
	HashTable<V,Hf,Eq>::HashTable(unsigned long ulSize, const Hf& hf, const Eq& eq):m_ulCurSize(0),m_bucket(NULL),m_ulFullSize(0)
	{
		init_bucket(ulSize);
	}

	template <class V, class Hf, class Eq >
	HashTable<V,Hf,Eq>::HashTable(unsigned long ulSize):m_ulCurSize(0),m_bucket(NULL),m_ulFullSize(0)
	{
		init_bucket(ulSize);
	}

	template <class V, class Hf, class Eq>
	HashTable<V,Hf,Eq>::~HashTable()
	{
		if(m_bucket != NULL)
			free(m_bucket);
	}

	template <class V>
	__hash_node<V>* create_node(const V& obj)
	{
		__hash_node<V>* p = new __hash_node<V>;
		p->next = NULL;
		p->val = obj;

		return p;
	}


	template <class V>
	void destory_node(__hash_node<V>* p)
	{
		delete p;
	}


	template <class V, class Hf, class Eq>
	void HashTable<V,Hf,Eq>::init_bucket(unsigned long ulSize)
	{
		m_ulFullSize = LowerBound(prime_list, number_of_primes, ulSize);
		m_bucket = (node**)malloc(sizeof(node*) * m_ulFullSize);
		memset(m_bucket,	0,	sizeof(node*) * m_ulFullSize);
	}


	template <class V, class Hf, class Eq>
	void HashTable<V,Hf,Eq>::resize(bool isUnique)
	{
		if(m_ulCurSize > m_ulFullSize && m_ulCurSize < prime_list[number_of_primes-1])
		{
			node**			oldbucket = NULL;
			node			*p = NULL,*q = NULL;
			unsigned long	ulSize = 0;

			//printf("resize() cursize:%ld , fullsize:%ld,\n", m_ulCurSize, m_ulFullSize);
			oldbucket = m_bucket;
			ulSize = m_ulFullSize;
			m_ulCurSize = 0;
			m_ulFullSize = LowerBound(prime_list, number_of_primes, m_ulFullSize + 1);
			m_bucket = (node**)malloc(sizeof(node*) * m_ulFullSize);
			memset(m_bucket,	0,	sizeof(node*) * m_ulFullSize);
			//将原来的所有元素移到新的bucket中
			for(unsigned long i=0; i< ulSize; i++)
			{
				if(oldbucket[i] == NULL)
					continue;
				q = oldbucket[i];
				do
				{
					if(isUnique)
						insert_unique_noresize(q->val);
					else
						insert_equal_noresize(q->val);
					p = q;
					q = q->next;
					destory_node(p);
				}while(q != NULL);
			}
			if(oldbucket != NULL)
				free(oldbucket);
		}
	}


	template <class V, class Hf, class Eq>
	int HashTable<V,Hf,Eq>::insert_unique_noresize(const V& obj)
	{
		unsigned long key = m_hash(obj, m_ulFullSize);
		node *p = NULL, *q = NULL;
		bool isExist = false;

		if(m_bucket[key] == NULL)
		{
			m_bucket[key] = create_node(obj);
		}
		else
		{
			q = m_bucket[key];
			do
			{
				if(m_equals(q->val, obj))
				{
					//已经存在相同的节点了。
					isExist = true;
					break;
				}

				p = q;
				q = q->next;
			}while(q != NULL);

			if(!isExist)
			{
				p->next = create_node(obj);
			}
		}

		m_ulCurSize ++;
		return 1;
	}

	template <class V, class Hf, class Eq>
	int HashTable<V,Hf,Eq>::insert_unique(const V& obj)
	{
		insert_unique_noresize(obj);
		resize(true);
		return 1;
	}

	template <class V, class Hf, class Eq>
	int HashTable<V,Hf,Eq>::insert_equal_noresize(const V& obj)
	{
		unsigned long key = m_hash(obj, m_ulFullSize);
		node *p = NULL, *q = NULL;

		p = create_node(obj);
		if(m_bucket[key] == NULL)
		{
			m_bucket[key] = p;
		}
		else
		{
			p->next = m_bucket[key]->next;
			m_bucket[key]->next = p;
		}

		m_ulCurSize ++;
		return 1;
	}

	template <class V, class Hf, class Eq>
	int HashTable<V,Hf,Eq>::insert_equal(const V& obj)
	{
		insert_equal_noresize(obj);
		resize(false);
		return 1;
	}

	template <class V, class Hf, class Eq>
	const V* HashTable<V,Hf,Eq>::find(const V& obj)
	{
		unsigned long key = m_hash(obj, m_ulFullSize);
		node *p = NULL;

		if(m_bucket[key] != NULL)
		{
			p = m_bucket[key];
			do
			{

				if(m_equals(p->val, obj))
					return &obj;
				p = p->next;
			}while(p != NULL);
		}

		return NULL;
	}
}
#endif /* __HASHTABLE_H__ */
