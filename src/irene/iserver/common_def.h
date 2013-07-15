#ifndef common_def_H_
#define common_def_H_

#include <stdint.h>
#include <functional>
#include <memory>
#include <thread>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_RECV_LEN    (1024 * 4)

namespace irene
{
namespace net_params
{
    inline int32_t smart_thread_nums()
    {
#ifdef _WIN32
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return si.dwNumberOfProcessors;
#else
        return sysconf(_SC_NPROCESSORS_CONF);
#endif
    }

    inline uint32_t max_recv_length() { return MAX_RECV_LEN; }
}
}

//types
#ifndef byte
    typedef unsigned char byte;
#endif

//endianness
template <typename T16>
static T16 swap16(const T16& v)
{
    return ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff));
}

template <typename T32>
static T32 swap32(const T32& v)
{
    return (((v) >> 24) | \
           (((v) & 0x00ff0000) >> 8)  | \
           (((v) & 0x0000ff00) << 8)  | \
           ((v) << 24));
}

template <typename T64>
static T64 swap64(const T64& v)
{
    return (((v) >> 56) | \
            (((v) & 0x00ff000000000000) >> 40) | \
            (((v) & 0x0000ff0000000000) >> 24) | \
            (((v) & 0x000000ff00000000) >> 8)  | \
            (((v) & 0x00000000ff000000) << 8)  | \
            (((v) & 0x0000000000ff0000) << 24) | \
            (((v) & 0x000000000000ff00) << 40) | \
            (((v) << 56)));
}

static float swapfloat(const float& p)
{
	union { float asfloat; byte asbytes[4]; } u1, u2;
	u1.asfloat = p;
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	//*p = u2.asfloat;
    return u2.asfloat;
}

static double swapdouble(const double& p)
{
	union { double asdouble; byte asbytes[8]; } u1, u2;
	u1.asdouble = p;
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	//*p = u2.asfloat;
    return u2.asdouble;
}

//forward declarations 
class TcpConnection;
class InetAddress;
class ByteBuffer;

//smart ptr
typedef std::shared_ptr<std::thread> ThreadPtr;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::shared_ptr<ByteBuffer> ByteBufferPtr;

//callbacks
typedef std::function<void (const TcpConnectionPtr& connection, uint32_t bytes_transferred)> WriteCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, const byte& buffer, uint32_t bytes_transferred)> ReadCompletedCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> AcceptedCallback;
typedef std::function<void (const TcpConnectionPtr& connection, const InetAddress& address)> NewConnectionCallback;
typedef std::function<void (const TcpConnectionPtr& connection)> ConnectionClosedCallback;

#endif