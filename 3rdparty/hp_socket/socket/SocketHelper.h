/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.2.3
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <malloc.h>

#include "../GeneralHelper.h"
#include "../CriticalSection.h"

/************************************************************************
名称：连接 ID 数据类型
描述：应用程序可以把 CONNID 定义为只身需要的类型（如：ULONG / ULONGLONG）
************************************************************************/
typedef ULONG_PTR	CONNID;

/*****************************************************************************************************/
/******************************************** 公共类、接口 ********************************************/
/*****************************************************************************************************/


/************************************************************************
名称：Windows Socket 组件初始化类
描述：自动加载和卸载 Windows Socket 组件
************************************************************************/
class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = nullptr, BYTE minorVersion = 2, BYTE majorVersion = 2)
	{
		LPWSADATA lpTemp = lpWSAData;
		if(!lpTemp)
			lpTemp	= (LPWSADATA)_alloca(sizeof(WSADATA));

		m_iResult	= ::WSAStartup(MAKEWORD(minorVersion, majorVersion), lpTemp);
	}

	~CInitSocket()
	{
		if(IsValid())
			::WSACleanup();
	}

	int		GetResult()	{return m_iResult;}
	BOOL	IsValid()	{return m_iResult == 0;}

private:
	int		m_iResult;
};

/************************************************************************
名称：通信组件服务状态
描述：应用程序可以通过通信组件的 GetState() 方法获取组件当前服务状态
************************************************************************/
enum EnServiceState
{
	SS_STARTING	= 0,	// 正在启动
	SS_STARTED	= 1,	// 已经启动
	SS_STOPING	= 2,	// 正在停止
	SS_STOPED	= 3,	// 已经启动
};

/************************************************************************
名称：Socket 操作类型
描述：应用程序的 OnErrror() 事件中通过该参数标识是哪种操作导致的错误
************************************************************************/
enum EnSocketOperation
{
	SO_UNKNOWN	= 0,	// Unknown
	SO_ACCEPT	= 1,	// Acccept
	SO_CONNECT	= 2,	// Connnect
	SO_SEND		= 3,	// Send
	SO_RECEIVE	= 4,	// Receive
};

/************************************************************************
名称：Socket 监听器基接口
描述：定义服务端和客户端 Socket 监听器的公共信息
************************************************************************/
class ISocketListener
{
public:
	/************************************************************************
	名称：事件通知处理结果
	描述：事件通知的返回值，不同的返回值会影响通信组件的后续行为
	************************************************************************/
	enum EnHandleResult
	{
		HR_OK		= 0,	// 成功
		HR_IGNORE	= 1,	// 忽略
		HR_ERROR	= 2,	// 错误
	};

public:

	/*
	* 名称：已发送数据通知
	* 描述：成功发送数据后，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 已发送数据缓冲区
	*			iLength		-- 已发送数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 该通知不允许返回 HR_ERROR（调试模式下引发断言错误）
	*/
	virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength)					= 0;

	/*
	* 名称：已接收数据通知（PUSH 模型）
	* 描述：对于 PUSH 模型的 Socket 通信组件，成功接收数据后将向 Socket 监听器发送该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 已接收数据缓冲区
	*			iLength		-- 已接收数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnError() 事件时间并关闭连接
	*/
	virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)				= 0;

	/*
	* 名称：数据到达通知（PULL 模型）
	* 描述：对于 PULL 模型的 Socket 通信组件，成功接收数据后将向 Socket 监听器发送该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			iLength		-- 已接收数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnError() 事件时间并关闭连接
	*/
	virtual EnHandleResult OnReceive(CONNID dwConnID, int iLength)									= 0;

	/*
	* 名称：已关闭连接通知
	* 描述：正常关闭连接后，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID -- 连接 ID
	* 返回值：	忽略返回值
	*/
	virtual EnHandleResult OnClose(CONNID dwConnID)													= 0;

	/*
	* 名称：通信错误通知
	* 描述：通信发生错误后，Socket 监听器将收到该通知，并关闭连接
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			enOperation	-- Socket 操作类型
	*			iErrorCode	-- 错误代码
	* 返回值：	忽略返回值
	*/
	virtual EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)	= 0;

public:
	virtual ~ISocketListener() {}
};

/************************************************************************
名称：服务端 Socket 监听器接口
描述：定义服务端 Socket 监听器的所有事件通知
************************************************************************/
class IServerSocketListener : public ISocketListener
{
public:

	/*
	* 名称：准备监听通知
	* 描述：通信服务端组件启动时，在监听 Socket 创建完成并开始执行监听前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*		
	* 参数：		soListen	-- 监听 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信服务组件
	*/
	virtual EnHandleResult OnPrepareListen(SOCKET soListen)				= 0;

	/*
	* 名称：接收连接通知
	* 描述：接收到客户端连接请求时，Socket 监听器将收到该通知，监听器可以在通知处理方
	*		法中执行 Socket 选项设置或拒绝客户端连接等额外工作
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			soClient	-- 客户端 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 接受连接
	*			HR_ERROR			-- 拒绝连接
	*/
	virtual EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient)	= 0;

	/*
	* 名称：关闭通信组件通知
	* 描述：通信组件关闭时，Socket 监听器将收到该通知
	*		
	* 参数：	
	* 返回值：忽略返回值
	*/
	virtual EnHandleResult OnServerShutdown()							= 0;
};

/************************************************************************
名称：PUSH 模型服务端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CServerSocketListener : public IServerSocketListener
{
public:
	virtual EnHandleResult OnReceive(CONNID dwConnID, int iLength)					{return HR_IGNORE;}
	virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}
	virtual EnHandleResult OnPrepareListen(SOCKET soListen)							{return HR_IGNORE;}
	virtual EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient)				{return HR_IGNORE;}
	virtual EnHandleResult OnServerShutdown()										{return HR_IGNORE;}
};

/************************************************************************
名称：PULL 模型服务端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CPullServerSocketListener : public CServerSocketListener
{
public:
	virtual EnHandleResult OnReceive(CONNID dwConnID, int iLength)						= 0;
	virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}
};

/************************************************************************
名称：客户端 Socket 监听器接口
描述：定义客户端 Socket 监听器的所有事件通知
************************************************************************/
class IClientSocketListener : public ISocketListener
{
public:

	/*
	* 名称：准备连接通知
	* 描述：通信客户端组件启动时，在客户端 Socket 创建完成并开始执行连接前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			socket		-- 客户端 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信客户端组件
	*/
	virtual EnHandleResult OnPrepareConnect(CONNID dwConnID, SOCKET socket)		= 0;

	/*
	* 名称：连接完成通知
	* 描述：与服务端成功建立连接时，Socket 监听器将收到该通知
	*		
	* 参数：		dwConnID	-- 连接 ID
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 同步连接：终止启动通信客户端组件
	*								   异步连接：关闭连接
	*/
	virtual EnHandleResult OnConnect(CONNID dwConnID)							= 0;
};

/************************************************************************
名称：PUSH 模型客户端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CClientSocketListener : public IClientSocketListener
{
public:
	virtual EnHandleResult OnReceive(CONNID dwConnID, int iLength)					{return HR_IGNORE;}
	virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}
	virtual EnHandleResult OnPrepareConnect(CONNID dwConnID, SOCKET socket)			{return HR_IGNORE;}
	virtual EnHandleResult OnConnect(CONNID dwConnID)								{return HR_IGNORE;}
};

/************************************************************************
名称：PULL 客户端 Socket 监听器抽象基类
描述：定义某些事件通知的默认处理方法（忽略事件）
************************************************************************/
class CPullClientSocketListener : public CClientSocketListener
{
public:
	virtual EnHandleResult OnReceive(CONNID dwConnID, int iLength)						= 0;
	virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)	{return HR_IGNORE;}
};

/************************************************************************
名称：通信服务端组件接口
描述：定义通信服务端组件的所有操作方法和属性访问方法
************************************************************************/
class IServerSocket
{
public:

	/************************************************************************
	名称：操作结果代码
	描述：Start() / Stop() 方法执行失败时，可通过 GetLastError() 获取错误代码
	************************************************************************/
	enum En_ISS_Error
	{
		ISS_OK						= 0,	// 成功
		ISS_ILLEGAL_STATE			= 1,	// 当前状态不允许操作
		ISS_INVALID_PARAM			= 2,	// 非法参数
		ISS_SOCKET_CREATE			= 3,	// 创建监听 SOCKET 失败
		ISS_SOCKET_BIND				= 4,	// 绑定监听地址失败
		ISS_SOCKET_PREPARE			= 5,	// 设置监听 SOCKET 失败
		ISS_SOCKET_LISTEN			= 6,	// 启动监听失败
		ISS_CP_CREATE				= 7,	// 创建完成端口失败
		ISS_WORKER_THREAD_CREATE	= 8,	// 创建工作线程失败
		ISS_SOCKE_ATTACH_TO_CP		= 9,	// 监听 SOCKET 绑定到完成端口失败
		ISS_ACCEPT_THREAD_CREATE	= 10,	// 创建监听线程失败
	};

public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动服务端通信组件，启动完成后可开始接收客户端连接并收发数据
	*		
	* 参数：		pszBindAddress	-- 监听地址
	*			usPort			-- 监听端口
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start	(LPCTSTR pszBindAddress, USHORT usPort)								= 0;

	/*
	* 名称：关闭通信组件
	* 描述：关闭服务端通信组件，关闭完成后断开所有客户端连接并释放所有资源
	*		
	* 参数：	
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop	()																	= 0;

	/*
	* 名称：发送数据
	* 描述：用户通过该方法向指定客户端发送数据
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pBuffer		-- 发送数据缓冲区
	*			iLength		-- 发送数据长度
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL Send	(CONNID dwConnID, const BYTE* pBuffer, int iLength)					= 0;

	/*
	* 名称：断开连接
	* 描述：断开与某个客户端的连接
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			bForce		-- 是否强制断开连接
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL Disconnect(CONNID dwConnID, BOOL bForce = TRUE)							= 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/*
	* 名称：设置连接的附加数据
	* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pv			-- 数据
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败（无效的连接 ID）
	*/
	virtual BOOL SetConnectionExtra		(CONNID dwConnID, PVOID pExtra)						= 0;

	/*
	* 名称：获取连接的附加数据
	* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序只身决定
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			ppv			-- 数据指针
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败（无效的连接 ID）
	*/
	virtual BOOL GetConnectionExtra		(CONNID dwConnID, PVOID* ppExtra)					= 0;

	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted				()													= 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState GetState		()													= 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual En_ISS_Error GetLastError	()													= 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR		GetLastErrorDesc()													= 0;
	/* 获取监听 Socket 的地址信息 */
	virtual BOOL GetListenAddress(CString& strAddress, USHORT& usPort)						= 0;
	/* 获取某个客户端连接的地址信息 */
	virtual BOOL GetClientAddress(CONNID dwConnID, CString& strAddress, USHORT& usPort)		= 0;

	/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
	virtual void SetWorkerThreadCount	(DWORD dwWorkerThreadCount)		= 0;
	/* 设置 Accept 预投递 Socket 数量（通常设置为工作线程数的 1 - 2 倍） */
	virtual void SetAcceptSocketCount	(DWORD dwAcceptSocketCount)		= 0;
	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)		= 0;
	/* 设置监听 Socket 的等候队列大小（根据并发连接数量调整设置） */
	virtual void SetSocketListenQueue	(DWORD dwSocketListenQueue)		= 0;
	/* 设置 Socket 缓存池大小（通常设置为平均并发连接数量的 1/3 - 1/2） */
	virtual void SetFreeSocketObjPool	(DWORD dwFreeSocketObjPool)		= 0;
	/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
	virtual void SetFreeBufferObjPool	(DWORD dwFreeBufferObjPool)		= 0;
	/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
	virtual void SetFreeSocketObjHold	(DWORD dwFreeSocketObjHold)		= 0;
	/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
	virtual void SetFreeBufferObjHold	(DWORD dwFreeBufferObjHold)		= 0;
	/* 设置心跳包间隔（毫秒，0 则不发送心跳包） */
	virtual void SetKeepAliveTime		(DWORD dwKeepAliveTime)			= 0;
	/* 设置心跳确认包检测间隔（毫秒，0 不发送心跳包，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)		= 0;
	/* 设置关闭服务前等待连接关闭的最长时限（毫秒，0 则不等待） */
	virtual void SetMaxShutdownWaitTime	(DWORD dwMaxShutdownWaitTime)	= 0;

	/* 获取工作线程数量 */
	virtual DWORD GetWorkerThreadCount	()	= 0;
	/* 获取 Accept 预投递 Socket 数量 */
	virtual DWORD GetAcceptSocketCount	()	= 0;
	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* 获取监听 Socket 的等候队列大小 */
	virtual DWORD GetSocketListenQueue	()	= 0;
	/* 获取 Socket 缓存池大小 */
	virtual DWORD GetFreeSocketObjPool	()	= 0;
	/* 获取内存块缓存池大小 */
	virtual DWORD GetFreeBufferObjPool	()	= 0;
	/* 获取 Socket 缓存池回收阀值 */
	virtual DWORD GetFreeSocketObjHold	()	= 0;
	/* 获取内存块缓存池回收阀值 */
	virtual DWORD GetFreeBufferObjHold	()	= 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetKeepAliveTime		()	= 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetKeepAliveInterval	()	= 0;
	/* 获取关闭服务前等待连接关闭的最长时限 */
	virtual DWORD GetMaxShutdownWaitTime()	= 0;

public:
	virtual ~IServerSocket() {}
};

/* 通信服务端组件接口智能指针 */
typedef auto_ptr<IServerSocket>	IServerSocketPtr;

/************************************************************************
名称：通信服务端组件接口
描述：定义通信客户端组件的所有操作方法和属性访问方法
************************************************************************/
class IClientSocket
{
public:
	/************************************************************************
	名称：操作结果代码
	描述：Start() / Stop() 方法执行失败时，可通过 GetLastError() 获取错误代码
	************************************************************************/
	enum En_ISC_Error
	{
		ISC_OK						= 0,	// 成功
		ISC_ILLEGAL_STATE			= 1,	// 当前状态不允许操作
		ISC_INVALID_PARAM			= 2,	// 非法参数
		ISC_SOCKET_CREATE_FAIL		= 3,	// 创建 Client Socket 失败
		ISC_SOCKET_PREPARE_FAIL		= 4,	// 设置 Client Socket 失败
		ISC_CONNECT_SERVER_FAIL		= 5,	// 连接服务器失败
		ISC_WORKER_CREATE_FAIL		= 6,	// 创建工作线程失败
		ISC_NETWORK_ERROR			= 7,	// 网络错误
		ISC_DATA_PROC_ERROR			= 8,	// 数据处理错误
	};

public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
	*		
	* 参数：		pszRemoteAddress	-- 服务端地址
	*			usPort				-- 服务端端口
	*			bAsyncConnect		-- 是否采用异步 Connnect
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start	(LPCTSTR pszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = FALSE)	= 0;

	/*
	* 名称：关闭通信组件
	* 描述：关闭客户端通信组件，关闭完成后断开与服务端的连接并释放所有资源
	*		
	* 参数：	
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop	()																		= 0;

	/*
	* 名称：发送数据
	* 描述：用户通过该方法向服务端发送数据
	*		
	* 参数：		dwConnID	-- 连接 ID（保留参数，目前该参数并未使用）
	*			pBuffer		-- 发送数据缓冲区
	*			iLength		-- 发送数据长度
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Send	(CONNID dwConnID, const BYTE* pBuffer, int iLength)						= 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/


	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted					()													= 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState	GetState		()													= 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual En_ISC_Error	GetLastError	()													= 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR			GetLastErrorDesc()													= 0;
	/* 获取该组件对象的连接 ID */
	virtual CONNID			GetConnectionID	()													= 0;
	/* 获取 Client Socket 的地址信息 */
	virtual BOOL			GetLocalAddress	(CString& strAddress, USHORT& usPort)				= 0;

	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为：(N * 1024) - sizeof(TBufferObj)） */
	virtual void SetSocketBufferSize	(DWORD dwSocketBufferSize)	= 0;
	/* 设置心跳包间隔（毫秒，0 则不发送心跳包） */
	virtual void SetKeepAliveTime		(DWORD dwKeepAliveTime)		= 0;
	/* 设置心跳确认包检测间隔（毫秒，0 不发送心跳包，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
	virtual void SetKeepAliveInterval	(DWORD dwKeepAliveInterval)	= 0;

	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize	()	= 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetKeepAliveTime		()	= 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetKeepAliveInterval	()	= 0;

public:
	virtual ~IClientSocket() {}
};

/* 通信客户端组件接口智能指针 */
typedef auto_ptr<IClientSocket>	IClientSocketPtr;

/************************************************************************
名称：PULL 模型组件接口
描述：定义 PULL 模型组件的所有操作方法
************************************************************************/
class IPullSocket
{
public:
	/************************************************************************
	名称：数据抓取结果
	描述：数据抓取操作的返回值
	************************************************************************/
	enum EnFetchResult
	{
		FR_OK				= 0,	// 成功
		FR_LENGTH_TOO_LONG	= 1,	// 抓取长度过大
		FR_DATA_NOT_FOUND	= 2,	// 找不到 ConnID 对应的数据
	};

public:
	/*
	* 名称：抓取数据
	* 描述：用户通过该方法从 Socket 组件中抓取数据
	*		
	* 参数：		dwConnID	-- 连接 ID
	*			pBuffer		-- 数据抓取缓冲区
	*			iLength		-- 抓取数据长度
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual EnFetchResult Fetch	(CONNID dwConnID, BYTE* pBuffer, int iLength)	= 0;

public:
	virtual ~IPullSocket() {}
};

/************************************************************************
名称：PULL 模型通信服务端组件接口
描述：继承了 IServerSocket 和 IPullSocket
************************************************************************/
class IPullServerSocket : public IPullSocket
{
	/* 设置 PULL 缓存池大小（通常设置为与 Socket 缓存池大小一致） */
	virtual void SetFreePullBufferPool	(DWORD dwFreePullBufferPool)	= 0;
	/* 设置 PULL 缓存池回收阀值（通常设置为与 Socket 缓存池回收阀值一致） */
	virtual void SetFreePullBufferHold	(DWORD dwFreePullBufferHold)	= 0;
	/* 获取 PULL 缓存池大小 */
	virtual DWORD GetFreePullBufferPool	()								= 0;
	/* 获取 PULL 缓存池回收阀值 */
	virtual DWORD GetFreePullBufferHold	()								= 0;
};

/* PULL 模型通信服务端组件接口智能指针 */
typedef auto_ptr<IPullServerSocket>	IPullServerSocketPtr;

/************************************************************************
名称：PULL 模型通信客户端组件接口
描述：继承了 IClientSocket 和 IPullSocket
************************************************************************/
class IPullClientSocket : public IPullSocket
{

};

/* PULL 模型通信客户端组件接口智能指针 */
typedef auto_ptr<IPullClientSocket>	IPullClientSocketPtr;

/* 数据缓冲区基础结构 */
struct TBufferObjBase
{
	OVERLAPPED			ov;
	WSABUF				buff;
	EnSocketOperation	operation;
};

/* 数据缓冲区结构 */
struct TBufferObj : public TBufferObjBase
{
	SOCKET	client;
};

/* 数据缓冲区结构链表 */
typedef list<TBufferObj*>	TBufferObjPtrList;

/* Socket 缓冲区基础结构 */
struct TSocketObjBase
{
	SOCKET	socket;
};

/* 数据缓冲区结构 */
struct TSocketObj : public TSocketObjBase
{
	SOCKADDR_IN		clientAddr;
	CONNID			connID;
	PVOID			extra;
	CCriSec2		crisec;
};

/* 数据缓冲区结构链表 */
typedef list<TSocketObj*>					TSocketObjPtrList;
/* 数据缓冲区结构哈希表 */
typedef hash_map<CONNID, TSocketObj*>		TSocketObjPtrMap;
/* 数据缓冲区结构哈希表迭代器 */
typedef TSocketObjPtrMap::iterator			TSocketObjPtrMapI;
/* 数据缓冲区结构哈希表 const 迭代器 */
typedef TSocketObjPtrMap::const_iterator	TSocketObjPtrMapCI;
/* CBufferPtr 自动回收列表 */
typedef VectorWrapper<list<CBufferPtr*>, PtrSet_Cleaner<list<CBufferPtr*>>> CAutoBufferPtrList;

/* PULL 缓冲区结构 */
struct TPullBuffer
{
	CONNID connID;
	int length;
	CAutoBufferPtrList buffers;
	CCriSec cs;

	void Reset()
	{
		connID = 0;
		length = 0;
		buffers.Clear();
	}

	TPullBuffer(CONNID id = 0, int len = 0) : connID(id), length(len) {}
};

/* PULL 缓冲区结构链表 */
typedef list<TPullBuffer*>					TPullBufferPtrList;
/* PULL 缓冲区结构哈希表 */
typedef hash_map<CONNID, TPullBuffer*>		TPullBufferPtrMap;
/* PULL 缓冲区结构哈希表迭代器 */
typedef TPullBufferPtrMap::iterator			CAutoPullBufferPtrMapI;
/* PULL 缓冲区结构哈希表 const 迭代器 */
typedef TPullBufferPtrMap::const_iterator	CAutoPullBufferPtrMapCI;

/*****************************************************************************************************/
/******************************************** 公共帮助方法 ********************************************/
/*****************************************************************************************************/

/* 检查字符串是否符合 IP 地址格式 */
BOOL IsIPAddress(LPCTSTR lpszAddress);
/* 通过主机名获取 IP 地址 */
BOOL GetIPAddress(LPCTSTR lpszHost, CString& strIP);
/* 把 SOCKADDR_IN 结构转换为地址数据 */
BOOL sockaddr_IN_2_A(const SOCKADDR_IN& addr, ADDRESS_FAMILY& usFamily, CString& strAddress, USHORT& usPort);
/* 把地址数据转换为 SOCKADDR_IN 结构 */
BOOL sockaddr_A_2_IN(ADDRESS_FAMILY usFamily, LPCTSTR pszAddress, USHORT usPort, SOCKADDR_IN& addr);
/* 获取 Socket 的本地或远程地址信息 */
BOOL GetSocketAddress(SOCKET socket, CString& strAddress, USHORT& usPort, BOOL bLocal = TRUE);
/* 获取 Socket 的本地地址信息 */
BOOL GetSocketLocalAddress(SOCKET socket, CString& strAddress, USHORT& usPort);
/* 获取 Socket 的远程地址信息 */
BOOL GetSocketRemoteAddress(SOCKET socket, CString& strAddress, USHORT& usPort);

/* 获取 Socket 的某个扩展函数的指针 */
PVOID GetExtensionFuncPtr					(SOCKET sock, GUID guid);
/* 获取 AcceptEx 扩展函数指针 */
LPFN_ACCEPTEX Get_AcceptEx_FuncPtr			(SOCKET sock);
/* 获取 GetAcceptExSockaddrs 扩展函数指针 */
LPFN_GETACCEPTEXSOCKADDRS Get_GetAcceptExSockaddrs_FuncPtr(SOCKET sock);
/* 获取 ConnectEx 扩展函数指针 */
LPFN_CONNECTEX Get_ConnectEx_FuncPtr		(SOCKET sock);
/* 获取 TransmitFile 扩展函数指针 */
LPFN_TRANSMITFILE Get_TransmitFile_FuncPtr	(SOCKET sock);
/* 获取 DisconnectEx 扩展函数指针 */
LPFN_DISCONNECTEX Get_DisconnectEx_FuncPtr	(SOCKET sock);

/************************************************************************
名称：setsockopt() 帮助方法
描述：简化常用的 setsockopt() 调用
************************************************************************/

int SSO_UpdateAcceptContext	(SOCKET soClient, SOCKET soBind);
int SSO_UpdateConnectContext(SOCKET soClient, int iOption);
int SSO_NoDelay				(SOCKET sock, BOOL bNoDelay = TRUE);
int SSO_DontLinger			(SOCKET sock, BOOL bDont = TRUE);
int SSO_Linger				(SOCKET sock, USHORT l_onoff, USHORT l_linger);
int SSO_KeepAlive			(SOCKET sock, BOOL bKeepAlive = TRUE);
int SSO_KeepAliveVals		(SOCKET sock, u_long onoff, u_long time, u_long interval);
int SSO_RecvBuffSize		(SOCKET sock, int size);
int SSO_SendBuffSize		(SOCKET sock, int size);
int SSO_ReuseAddress		(SOCKET sock, BOOL bReuse = TRUE);

/************************************************************************
名称：Socket 操作方法
描述：Socket 操作包装方法
************************************************************************/

/* 生成 Connection ID */
CONNID GenerateConnectionID	(volatile CONNID& dwSeed);
/* 关闭 Socket */
int ManualCloseSocket		(SOCKET sock, int iShutdownFlag = 0xFF, BOOL bGraceful = TRUE, BOOL bReuseAddress = FALSE);
/* 投递 AccceptEx() */
int PostAccept				(LPFN_ACCEPTEX pfnAcceptEx, SOCKET soListen, SOCKET soClient, TBufferObj* pBufferObj);
/* 投递 WSASend() */
int PostSend				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);
/* 投递 WSARecv() */
int PostReceive				(TSocketObj* pSocketObj, TBufferObj* pBufferObj);