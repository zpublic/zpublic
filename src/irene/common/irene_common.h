#ifndef COMMON_H_
#define COMMON_H_

#ifdef _WIN32
	#define SERVER_DECL __declspec(dllexport)
#endif

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <functional>
#include <thread>
#include <iostream>
#include <stdint.h>
#include <list>
#include <queue>
#include <vector>
#include <map>
#include <hash_map>

#include <google/protobuf/message.h>

//types
#ifndef byte
    typedef unsigned char byte;
#endif

#endif