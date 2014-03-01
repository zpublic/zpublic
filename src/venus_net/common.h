#ifndef __COMMON_H_
#define __COMMON_H_

#if defined(_WIN32)
    #define SERVER_DECL __declspec(dllexport)
#endif

// types
#if defined(_WIN32)	//for windows
#ifndef byte
typedef unsigned __int8    byte;
#endif
typedef __int8             int8;
typedef __int16            int16;
typedef __int32            int32;
typedef __int64            int64;
typedef unsigned __int32   uint;
typedef unsigned __int8    uint8;
typedef unsigned __int16   uint16;
typedef unsigned __int32   uint32;
typedef unsigned __int64   uint64;
#else
#ifndef byte
typedef unsigned char      byte;
#endif
typedef char               int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned int       uint;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
#endif

// lines
#if defined(_WIN32)
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif

// safe delete macro
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if (nullptr != (x)) { delete (x); (x) = nullptr; }
#endif

#ifndef SAFE_DELETE_ARR
#define SAFE_DELETE_ARR(x)	if (nullptr != (x)) { delete [] (x); (x) = nullptr; }
#endif

#ifndef RETURN_IF_NULLPTR
#define RETURN_IF_NULLPTR(ptr, log) if (nullptr == ptr){ error_log(log); return; }
#endif

#ifndef RETURN_IF_NULLPTR_WITH_RESULT
#define RETURN_IF_NULLPTR_WITH_RESULT(ptr, log, result) if (nullptr == ptr){ error_log(log); return result; }
#endif

#ifndef GETTER_SETTER_MEMBER
#define GETTER_SETTER_MEMBER(type, name) \
    private: type _##name; \
    public: type name() const {return this->m_##name;} \
    public: void name(type _arg){this->m_##name=_arg;} \
    private:
#endif

//platform
#define ZEUS_EVENT

// boost
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

// std
#include <string>
#include <functional>
#include <thread>
#include <iostream>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <map>
#include <hash_map>
#include <mutex>

// google
#include <google/protobuf/message.h>

// adaptive map
#if defined(_WIN32)
#include <unordered_map>
template <typename Key, typename Value>
class adap_map : public std::unordered_map<Key, Value> {};
#else
template <typename Key, typename Value>
class adap_map : public std::map<Key, Value> {};
#endif

// venus net
#include "singleton.h"
#include "server_logger.h"

#endif