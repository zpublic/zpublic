#pragma once
#include "z_net_header.h"

typedef unsigned long ipaddr_t;
typedef unsigned short port_t;
typedef int socklen_t;
#ifndef uint64_t
typedef unsigned __int64 uint64_t;
#endif
#ifndef int64_t
typedef __int64 int64_t;
#endif
typedef std::list<SOCKET> socket_v;
