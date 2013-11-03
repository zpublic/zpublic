#ifndef OPCODES_H_
#define OPCODES_H_

#include "common.h"

namespace Opcodes
{
    //机器人鉴权
    const uint32 C2SLoginReq     = 10000;
    const uint32 S2CLoginRsp     = 10001;
    const uint32 C2SRegisterReq  = 10002;            //注册请求
    const uint32 S2CRegisterRsp  = 10003;            //注册回应
}

#endif