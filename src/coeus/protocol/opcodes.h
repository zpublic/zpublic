// Generate by the sword of core(核心之刃). DO NOT EDIT!
// Source : opcodes.h
// Comment : 在此处输入一些备注信息...

#ifndef __OPCODES_H__
#define __OPCODES_H__

#include <venus_net/network_common.h>

namespace Opcodes
{
    //注册登录(2 message)
    const static uint16 CSLoginReq = 10001;   //登录请求
    const static uint16 SCLoginRsp = 10002;   //登录返回
    //背包(4 message)
    const static uint16 CSBagList   = 20001;   //背包列表请求
    const static uint16 SCBagList   = 20101;   //背包列表返回
    const static uint16 CSBagResort = 20002;   //整理背包请求
    const static uint16 SCBagResort = 20102;   //整理背包返回
    //英雄(卡牌)(0 message)
    //属性(0 message)
    //聊天(0 message)
    //任务(0 message)
    //战斗(0 message)
    //房间(0 message)
    //道具(0 message)
    //商店(0 message)
    //排行榜(0 message)
    //公告(0 message)
    //称号(0 message)
    //VIP(0 message)
    //test(2 message)
    const static uint16 CSTestPacketReq = 99991;
    const static uint16 SCTestPacketRsp = 99992;
}

#endif