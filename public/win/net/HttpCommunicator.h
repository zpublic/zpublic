/************************************************************************/
/* File: HttpCommunicator.h                                             */
/* Date: 2005-11-29 18:25                                               */
/* Auth: PengNing                                                       */
/* Comm: Communicate using http(base on windows internet API)           */
/************************************************************************/
#ifndef HTTP_COMMUNICATOR_H_
#define HTTP_COMMUNICATOR_H_

#include <Windows.h>
#include <WinInet.h>

//Define the max length of proxy string
const int cnProxyStrMaxLen      = 0x0080; 

//Define the max length of internet address
const int cnInetAddrMaxLen      = 0x0100;

//Define the max size of buffer receiving data
//that HttpCommunicator supports  
const size_t cnRecvBufMaxSize   = 0x1000;

/**
 * enum ENUM_PROXY_TYPE
 * Enumerate the type of proxy
 */
enum ENUM_PROXY_TYPE
{
    enumPROXY_TYPE_UNKNOWN      = 0x00,
    enumPROXY_TYPE_SOCK4        = 0x01, 
    enumPROXY_TYPE_SOCK5        = 0x02,
    enumPROXY_TYPE_HTTP         = 0x03
};

/**
 * enum ENUM_PROXY_METHOD
 * Enumerate how to use proxy
 *
 * enumPROXY_METHOD_DIRECT  --- no proxy
 * enumPROXY_METHOD_USE_IE  --- same to IE
 * enumPROXY_METHOD_CUSTOM  --- user custom
 */
enum ENUM_PROXY_METHOD
{
    enumPROXY_METHOD_DIRECT     = 0x00,
    enumPROXY_METHOD_USE_IE     = 0x01,
    enumPROXY_METHOD_CUSTOM     = 0x02
};

/**
 * struct st_proxy_info
 * For CHttpCommunicator to store proxy information
 *
 * nProxyMethod     --- The method for using proxy
 *                      see: ENUM_PROXY_METHOD
 * nProxyType       --- The type of proxy
 *                      see: ENUM_PROXY_TYPE
 * szProxyAddr      --- The address of proxy server
 * usProxyPort      --- The port for proxy service
 * szUserName       --- The user name for validating
 * szPassword       --- The password for validating
 */
typedef struct st_proxy_info
{
    ENUM_PROXY_METHOD   nProxyMethod;
    ENUM_PROXY_TYPE     nProxyType;
    TCHAR               szProxyAddr[cnInetAddrMaxLen];
    unsigned short      usProxyPort;
    TCHAR               szUserName[cnProxyStrMaxLen]; 
    TCHAR               szPassword[cnProxyStrMaxLen];

}ST_PROXY_INFO, *PST_PROXY_INFO;

/**
 * struct st_inet_addr
 * For storing the internet address
 */
typedef struct st_inet_addr
{
    TCHAR               szAddr[cnInetAddrMaxLen];
    unsigned short      usPort;
}ST_INET_ADDR, *PST_INET_ADDR;

/**
 * class CHttpCommunicator
 * Define a simple HTTP communicator, support GET and POST
 */
class CHttpCommunicator 
{
public:
    //The default constructor
	CHttpCommunicator();

    //The default destructor
	~CHttpCommunicator();

public:
    /**
     * Send "GET" request and receives data
     * param[IN]     pszGet         The URL of requested resource
     * param[OUT]    pvRecvBuf      The buffer to receive data
     * param[IN,OUT] dwRecvBufSize  The size of pvRecvBuf
     *
     * return If succeeded
     * -true    yes
     *  false   no
     */
    bool DoHttpGet(
        IN      const TCHAR*    pszGet,
        OUT     void*           pvRecvBuf,
        IN OUT  DWORD&          dwRecvBufSize
    );

    /**
     * Send "POST" request and receives data
     *
     * param[IN]     pszPost        The URL of requested resource
     * param[IN]     pvSendBuf      The buffer storing data to send
     * param[IN]     dwSendBufSize  The size to send
     * param[OUT]    pvRecvBuf      The buffer to receive data
     * param[IN,OUT] dwRecvBufSize  The size of pvRecvBuf
     *
     *return If succeeded
     * -true    yes
     *  false   no
     */
    bool DoHttpPost(
        IN      const TCHAR*    pszPost,
        IN      const void*	    pvSendBuf,
        IN      DWORD           dwSendBufSize,
        OUT     void*           pvRecvBuf,
        IN OUT  DWORD&          dwRecvBufSize
    );

public:
    inline bool SetProxyInfo(const ST_PROXY_INFO& stProxyInfo);
	
    inline bool SetDestination(const ST_INET_ADDR& stInetAddr);

    inline void SetConnectTimeout(unsigned long ulTimeout);

    inline void SetSendTimeout(unsigned long ulTimeout);
    
    inline void SetRecvTimeout(unsigned long ulTimeout);

private:
    CHttpCommunicator(const CHttpCommunicator&);

    CHttpCommunicator& operator=(const CHttpCommunicator&);

private:
    bool DoInternetConnect(
        OUT     HINTERNET*  phInetOpen, 
        OUT     HINTERNET*  phInetConnect
    );

    bool DoHttpDataReceive(
        IN      HINTERNET*  phInetRequest, 
        OUT     void*       pvRecv, 
        IN OUT  DWORD&      dwSize
    );

    bool AddHttpRequestHeader(
        IN      HINTERNET*  phInetRequest
    );

private:
    /**
     * struct st_timeout
     * For storing timeout values
     */
    typedef struct st_timeout
    {
        unsigned long   ulConnTimeout;
        unsigned long   ulSendTimeout;
        unsigned long   ulRecvTimeout;
    }ST_TIMEOUT, *PST_TIMEOUT;

private:
    //The proxy information
    ST_PROXY_INFO       m_stProxyInfo;

    //The address of HTTP server[IP + port]
    ST_INET_ADDR        m_stServerAddr;

    //The timeout while communicating
    ST_TIMEOUT          m_stTimeout;
};

inline bool CHttpCommunicator::SetProxyInfo(const ST_PROXY_INFO& stProxyInfo)
{
    memcpy(&m_stProxyInfo, &stProxyInfo, sizeof(ST_PROXY_INFO));
    return true;
}

inline bool CHttpCommunicator::SetDestination(const ST_INET_ADDR& stInetAddr)
{
    memcpy(&m_stServerAddr, &stInetAddr, sizeof(ST_INET_ADDR));
    return true;
}

inline void CHttpCommunicator::SetConnectTimeout(unsigned long ulTimeout)
{
    m_stTimeout.ulConnTimeout   = ulTimeout;
}

inline void CHttpCommunicator::SetSendTimeout(unsigned long ulTimeout)
{
    m_stTimeout.ulSendTimeout   = ulTimeout;
}

inline void CHttpCommunicator::SetRecvTimeout(unsigned long ulTimeout)
{
    m_stTimeout.ulRecvTimeout   = ulTimeout;
}

#endif//HTTP_COMMUNICATOR_H_

