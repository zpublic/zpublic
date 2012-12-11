/** 
* @file LTIOInterface.h
* @brief 败者树的输入输出接口
* @author BAO<baoderong@kingsoft.com>
* @date May-25-2010
*/

#ifndef LT_IO_INTERFACE_H
#define LT_IO_INTERFACE_H

/** 
* @class ILTInput
* @brief 败者树多路归并算法的每路输入接口
*/
template <class LTItem>
class ILTInput
{
public:
	/** 
	* @brief 获取本路数据的第一条记录
	*
	* @param 无
	* @return 成功返回0，失败返回-1，空的返回1
	*/
	virtual int First() = 0;

	/** 
	* @brief 获取本路数据的下一条记录
	*
	* @param 无
	* @return 成功返回0，失败返回-1，没有下一条记录返回1
	*/
	virtual int Next() = 0;

	/** 
	* @brief 获取本路数据的当前记录
	*
	* @param 无
	* @return 成功返回0，失败返回-1
	*/
	virtual int CurrentItem(LTItem &stItem) const = 0;

	/** 
	* @brief 比较本路数据的当前记录与piInput路的当前记录
	*
	* @param[in] ILTInput *piInput 要比较的记录的句柄
	* @return 相等返回0，本记录大于反回1，本记录小于返回-1
	*/
	virtual int Comp(ILTInput *piInput) const = 0;
};

/** 
* @class ILTInput
* @brief 败者树多路归并算法的每路输出接口
*/
template <class LTItem>
class ILTOutput
{
public:
	/** 
	* @brief 将记录输出
	*
	* @param[in] ILTInput *piInput 要输出的记录的句柄
	* @param[in] int nPos 当前输出的路数号
	* @return 相等返回0，本记录大于反回1，本记录小于返回-1
	*/
	virtual int Output(ILTInput<LTItem> *piInput, int nPos) = 0;
};

#endif
