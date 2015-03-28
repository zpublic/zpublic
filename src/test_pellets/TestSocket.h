#pragma once

#include "def.h"
#include "z_socket/z_socket_test_code.h"

class CTestSocket : public Suite
{
public:
    CTestSocket(void);
    ~CTestSocket(void);

    void testSocket()
    {
        z_socket_test_code();
    }
};