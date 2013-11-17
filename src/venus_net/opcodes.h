#ifndef __OPCODES_H__
#define __OPCODES_H__

namespace Opcodes
{
	const int C2SHeartbeat              = 500;				//心跳请求
    const int C2SLoginReq               = 10000;			//登录请求
    const int S2CLoginRsp               = 10001;			//登录回应
    const int C2SRegisterReq            = 10002;            //注册请求
    const int S2CRegisterRsp            = 10003;            //注册回应
    const int C2SGetPlayerProfileReq    = 15000;			//获取玩家资料请求
    const int S2CGetPlayerProfileRsp    = 15001;			//获取玩家资料回应
    const int C2SChatMessageReq         = 20000;			//聊天信息请求
    const int S2CChatMessageNotify      = 20001;			//聊天信息通知
    const int S2CError                  = 99990;            //错误通知
    const int S2CErrorEx                = 99991;            //扩展错误通知
    const int C2SRoomCreateReq          = 21001;            //房间创建请求
    const int S2CRoomCreateRsp          = 21002;			//房间创建回应
    const int C2SGetRoomListReq         = 21003;			//获取房间列表请求
    const int S2CGetRoomListRsp         = 21004;			//获取房间列表回应
    const int C2SRoomCancleReq          = 21005;
    const int S2CRoomCancleRsp          = 21006;
    const int C2SSRoomInfoChangeReq     = 21007;
    const int S2CSRoomInfoChangeRsp     = 21008;
    const int C2SEnterRoomReq           = 21009;
    const int S2CEnterRoomRsp           = 21010;
    const int C2SLeaveRoomReq           = 21011;
    const int S2CPlayerLeaveRoomNotify  = 21012;
    const int C2SKickPlayerReq          = 21013;
    const int S2CRoomKickedRsp          = 21014;
    const int S2CNewRoomAddRsp          = 21015;
}

#endif