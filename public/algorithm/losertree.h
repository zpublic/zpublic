/** 
* @file LoserTree.h
* @brief 败者树多路归并算法类定义
* @author BAO<baoderong@kingsoft.com>
* @date May-25-2010
*
* @reference 本类参考自廖兰君合库代码中的败者树算法
*/

#ifndef LOSERTREE_H
#define LOSERTREE_H

#include <vector>

#include "LTIOInterface.h"

#define SWAP2INT( a, b )	\
	do						\
{							\
	a = a + b;				\
	b = a - b;				\
	a = a - b;				\
} while(0);

/** 
* @class CTreeOfLoser
* @brief 败者树多路归并算法类
*/
template <class LTItem>
class CTreeOfLoser
{
	typedef enum tagKeyStatus
	{
		enumKey_Status_Max = 1,
		enumKey_Status_Nor = 0,
		enumKey_Status_Min = -1
	}KEY_STATUS; 

public:
	/** 
	* @brief 构造函数
	*
	* @param 无
	* @return 无
	*/
	CTreeOfLoser(void)
	{
		m_dwNum = 0;
		m_piOut = NULL;
		m_pnTree = NULL;
	}

	/** 
	* @brief 析构函数
	*
	* @param 无
	* @return 无
	*/
	~CTreeOfLoser(void)
	{
		clear();
	}


	/** 
	* @brief 将一路要归并的记录添加入败者树
	*
	* @param[in] ILTInput<LTItem> *pIter 要归并的记录
	* @return 成功返回0，失败返回-1，没有记录返回1
	*
	* @remarks 当该接口所指向的文件不含任何记录时（空文件）它将不添加入败者树
	*/
	int AddInput(
		ILTInput<LTItem> *pIter)
	{
		int nResult = -1;
		int nRetCode = -1;

		if (NULL == pIter)
		{
			goto Exit0;
		}

		m_dwNum ++;
		nRetCode = pIter->First();

		if (1 == nRetCode)
		{
			nResult = 1;
			goto Exit0;
		}
		else if (-1 == nRetCode)
		{
			goto Exit0;
		}
		else
		{

		}

		m_vecipInput.push_back(pIter);

		nResult = 0;

Exit0:
		return nResult;
	}

	/** 
	* @brief 将归并输出接口的记录添加入败者树
	*
	* @param[in] ILTInput<LTItem> *pIter 要归并的记录
	* @return 返回0
	*
	* @remarks 当此指针为NULL时，不输出排序结果
	*/
	int SetOutput(
		ILTOutput<LTItem>* piOut = NULL)
	{
		m_piOut = piOut;
		m_vecStat.resize(m_dwNum + 1);

		for (int i = 0; i < (int)m_dwNum; i ++)
		{
			m_vecStat[i] = enumKey_Status_Nor;
		}

		m_vecStat[m_dwNum] = enumKey_Status_Min;
		return 0;
	}

	/** 
	* @brief 获取当前要归并的路数
	*
	* @param 无
	* @return 归并的路数
	*/
	virtual DWORD GetNumber()
	{
		return (DWORD)m_vecipInput.size();
	}

	/** 
	* @brief 第dwIndex路记录是否为最大值
	* @details 当该路记录全部归并完，没有记录时，该的值为最大值
	*
	* @param[in] const DWORD dwIndex 记录路数
	* @return 归并的路数
	*/
	virtual bool IsMax(
		const DWORD dwIndex)
	{
		if (dwIndex >= m_vecStat.size() 
			|| m_vecStat[dwIndex] != enumKey_Status_Max)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	/** 
	* @brief 读取dwIndex的下一条记录
	* @details 如果该队列已经输出完毕，那么数组中记录为最大值
	*
	* @param[in] const DWORD dwIndex 记录路数
	* @return 归并的路数
	*/
	virtual int InputOneItem(
		const DWORD dwIndex)
	{
		int	nResult = -1;
		int nRetCode = -1;

		ILTInput<LTItem> *IterData = m_vecipInput[dwIndex];

		if (!IterData)
		{
			goto Exit0;
		}

		if (enumKey_Status_Max == m_vecStat[dwIndex])
		{
			goto Exit1;
		}

		nRetCode = IterData->Next();

		if (1 == nRetCode)
		{
			m_vecStat[dwIndex] = enumKey_Status_Max;
		}
		else if (-1 == nRetCode)
		{
			goto Exit0;
		}
		else
		{
			m_vecStat[dwIndex] = enumKey_Status_Nor;
		}

Exit1:
		nResult = 0;
Exit0:
		return nResult;
	}

	/** 
	* @brief 将dwIndex的当前记录输出
	*
	* @param[in] const DWORD dwIndex 记录路数
	* @return 归并的路数
	*/
	virtual int OutputOneItem(
		const DWORD dwIndex)
	{
		int nResult = -1;

		//m_vecSortedArray.push_back(dwIndex);

		if (m_piOut)
		{
			m_piOut->Output(m_vecipInput[dwIndex], dwIndex);
		}

		nResult = 0;
		return nResult;
	}

	/** 
	* @brief 将dwIndex的当前记录输出
	* @details 如果dwIndex >= m_dwNumber 那么他肯定是最小的
	*
	* @param[in] const DWORD dwIndexA 要比较的路数A
	* @param[in] const DWORD dwIndexB 要比较的路数B
	* @param[out] int *nResult 比较结果，相等返回0，A>B返回1,A<B返回-1
	* @return 成功返回0，失败返回-1
	*/
	virtual int	Compare(
		const DWORD dwIndexA,
		const DWORD dwIndexB,
		int *nResult)
	{
		int nFuResult = -1;
		int nRetCod = -1;

		if (NULL == nResult)
		{
			goto Exit0;
		}

		*nResult = 0;

		if (enumKey_Status_Nor == m_vecStat[dwIndexA] 
		&& enumKey_Status_Nor == m_vecStat[dwIndexB])
		{
			nRetCod = (m_vecipInput[dwIndexA])->Comp(m_vecipInput[dwIndexB]);
		}
		else
		{
			nRetCod = m_vecStat[dwIndexA] - m_vecStat[dwIndexB];
		}

		*nResult = nRetCod;

		nFuResult = 0;
Exit0:
		return nFuResult;	
	}
	
	/** 
	* @brief 清除所有缓存数据
	*
	* @param 无
	* @return 无
	*/
	void clear()
	{
		m_dwNum = 0;
		m_piOut = NULL;
		m_vecSortedArray.clear();
		m_vecStat.clear();
		m_vecipInput.clear();

		if (m_pnTree)
		{
			delete [] m_pnTree;
			m_pnTree = NULL;
		}
	}

	/** 
	* @brief 根据下一条记录，重新构造好败者树
	*
	* @param[in] DWORD dwIndex 要更新记录的路数
	* @return 成功返回0，失败返回-1
	*/
	int Adjust(
		DWORD dwIndex)
	{
		int nResult = -1;
		int nRetCode = 0;

		int nParentIndex = (m_dwNum + dwIndex) / 2;

		while (nParentIndex > 0)
		{
			nResult = Compare(dwIndex, m_pnTree[nParentIndex], &nRetCode);

			if (nResult)
			{
				goto Exit0;
			}

			if (nRetCode > 0)
			{
				SWAP2INT(dwIndex, m_pnTree[nParentIndex]);
			}

			nParentIndex = nParentIndex / 2;
		}

		m_pnTree[0] = dwIndex;

		nResult = 0;
Exit0:
		return nResult;
	}

	/** 
	* @brief 用败者树多路归并算法对数据排序
	*
	* @param 无
	* @return 成功返回0，失败返回-1
	*/
	int Make()
	{
		int nResult = -1;
		int nRetcode = -1;

		DWORD dwIndex = 0;

		if (m_pnTree)
		{
			delete []m_pnTree;
			m_pnTree = NULL;
		}

		m_pnTree = new int[m_dwNum];

		if (NULL == m_pnTree)
		{
			goto Exit0;
		}

		/// 创建败者树
		for (int i = 0; i < (int)m_dwNum; ++i)
		{
			m_pnTree[i] = m_dwNum;
		}

		for(int i = m_dwNum - 1; i >= 0; --i)
		{
			nRetcode = Adjust(i);

			if (nRetcode)
			{
				goto Exit0;
			}
		}

		/// 用败者树排序
		while ( !IsMax(m_pnTree[0]) )
		{
			dwIndex = m_pnTree[0];
			nRetcode = OutputOneItem(dwIndex);

			if (nRetcode)
			{
				goto Exit0;
			}

			nRetcode = InputOneItem(dwIndex);

			if (nRetcode)
			{
				goto Exit0;
			}

			nRetcode = Adjust(dwIndex);

			if (nRetcode)
			{
				goto Exit0;
			}
		}

		if (nRetcode)
		{
			goto Exit0;
		}

		nResult = 0;

Exit0:
		return nResult;
	}

private:
	DWORD m_dwNum;
	int *m_pnTree;
	ILTOutput<LTItem> *m_piOut;

	std::vector<int> m_vecSortedArray;

	std::vector<KEY_STATUS>	m_vecStat;
	std::vector<ILTInput<LTItem> *> m_vecipInput;	
};

#endif

