#pragma once
#include "z_net_def.h"

class ZLSocketAddress
{
public:
    virtual ~ZLSocketAddress() {}
    virtual bool operator==(ZLSocketAddress&) = 0;

    virtual void SetAddress(struct sockaddr *sa) = 0;
    virtual operator struct sockaddr *() = 0;
    virtual operator socklen_t() = 0;

    virtual void SetPort(port_t port) = 0;
    virtual port_t GetPort() = 0;

    virtual std::string Convert(bool include_port) = 0;
    virtual std::string Reverse() = 0;

    virtual int GetFamily() = 0;
    virtual bool IsValid() = 0;
};
