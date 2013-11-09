#ifndef __OPCODES_H__
#define __OPCODES_H__

namespace Opcodes
{
    //机器人鉴权
    const int C2SLoginReq               = 10000;
    const int S2CLoginRsp               = 10001;
    const int C2SRegisterReq            = 10002;            //注册请求
    const int S2CRegisterRsp            = 10003;            //注册回应
    const int C2SGetPlayerProfileReq    = 15000;
    const int S2CGetPlayerProfileRsp    = 15001;
    const int C2SChatMessageReq         = 20000;
    const int S2CChatMessageNotify      = 20001;
    const int S2CError                  = 99990;            //注册回应
    const int S2CErrorEx                = 99991;            //注册回应
}

#endif