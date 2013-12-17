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

#ifndef PROP
#define PROP(type, varname, name) \
	public: \
		type get##name() const { return varname; } \
		void set##name(type var) { varname = var; } \
	private: \
		type varname;
#endif

#ifndef READONLY_PROP
#define READONLY_PROP(type, varname, name) \
	public: \
		const type& get##name() const { return varname; } \
	private: \
		type varname;
#endif

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
#include <set>
#include <hash_map>
#include <mutex>
#include <utility>

// adaptive map
#if defined(_WIN32)
#include <unordered_map>
template <typename Key, typename Value>
class adap_map : public std::unordered_map<Key, Value> {};
#else
#include <map>
template <typename Key, typename Value>
class adap_map : public std::map<Key, Value> {};
#endif

#endif