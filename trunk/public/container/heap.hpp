/*
 * heap.hpp
 *
 *  Created on: 2012-11-21
 *      Author: kevin
 */

#ifndef HEAP_HPP__
#define HEAP_HPP__

#include "vector.hpp"

namespace zl
{
	template<class T>
	class Heap
	{
	public:
		Heap()
		{
		
		}

		~Heap()
		{

		}

		Heap(const CSimpleVector<T>& array)
		{
			m_array = array;
		}

	protected:
		void Swap(T& x, T& y)
		{
			T tmp = x;
			x = y;
			y = tmp;
		}

		void AdjustHeap(int i, int nLenth)
		{
			int nChild = i*2 + 1;
			while(nChild < nLenth)
			{
				//nChild指向较小的节点
				if(nChild < nLenth - 1 && m_array[nChild+1] < m_array[nChild])
					nChild++;
				if(m_array[nChild] < m_array[i])
				{
					Swap(m_array[nChild], m_array[i]);
					i = nChild;
					nChild = i*2 + 1;
				}
				else
					break;
			}
		}


	public:
		int Size() const
		{
			return m_array.GetSize();
		}

		void push(T value)
		{
			m_array.Add(value);
			int n = m_array.GetSize();
			if(n > 2)
			{
				AdjustHeap(n/2 - 1, n);
			}
			//print_heap();
		}

		bool pop(T& ret)
		{

			int n = m_array.GetSize();
            if (n == 1)
            {
                ret = m_array[0];
                m_array.RemoveAll();
                return true;
            }
			else if(n > 1)
			{
				ret = m_array[0];

				Swap(m_array[0], m_array[n-1]);
				m_array.RemoveAt(n-1);
				if(n > 2)
					AdjustHeap(0, n-1);
				return true;
			}

			return false;
		}

// 		void print_heap()
// 		{
// 			for(int i = 0; i < m_array.GetSize(); i++)
// 			{
// 				printf("%d ", m_array[i]);
// 			}
// 			printf("\n");
// 		}

	private:
		CSimpleVector<T> m_array;
	};
}

#endif // HEAP_HPP__