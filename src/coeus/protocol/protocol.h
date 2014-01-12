// Generate by the sword of core(核心之刃). DO NOT EDIT!
// Source : protocol.h
// Comment : 在此处输入注释...

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <venus_net/network_common.h>

namespace Protocol
{
    inline uint32 getStrListSize(vector<string>& strList) 
    {
        uint32 size = 0; 
        for (uint32 i = 0; i < strList.size(); i++) 
        { 
            size += strList[i].length() + 2; 
        }

        return size + 2;
    }

    template<class T> 
    inline uint32 getStructArrBytesSize(vector<T>& vt)
    {
        uint32 size = 0; 
        for (uint32 i = 0; i < vt.size(); i++) 
        { 
            size += vt[i].byteSize(); 
        }

        return 2 + size;
    }

    /**
    * ● 注册登录(2 message)
    */

    //登录请求
    struct CSLoginReq : public NetworkMessage
    {
        //登录帐号（邮箱地址）
        string account;
        //密码
        string password;

        int byteSize()
        {
            return (account.length() + 2) + (password.length() + 2);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << account;
            w << password;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> account;
            r >> password;
        }
    };

    //登录返回
    struct SCLoginRsp : public NetworkMessage
    {
        //登录结果
        uint16 login_result;
        //角色ID
        uint64 player_id;

        int byteSize()
        {
            return sizeof(login_result) + sizeof(player_id);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << login_result;
            w << player_id;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> login_result;
            r >> player_id;
        }
    };

    /**
    * ● 背包(4 message)
    */

    //背包列表请求
    struct CSBagList : public NetworkMessage
    {
        //背包类型(0 - 包裹，1 - 仓库)
        uint8 m_nBagType;

        int byteSize()
        {
            return sizeof(m_nBagType);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << m_nBagType;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> m_nBagType;
        }
    };

    //背包列表返回
    struct SCBagList : public NetworkMessage
    {
        //格子数量
        uint8 m_nCellNum;

        int byteSize()
        {
            return sizeof(m_nCellNum);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << m_nCellNum;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> m_nCellNum;
        }
    };

    //整理背包请求
    struct CSBagResort : public NetworkMessage
    {
        //背包类型(0 - 包裹，1 - 仓库)
        uint8 m_nBagType;

        int byteSize()
        {
            return sizeof(m_nBagType);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << m_nBagType;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> m_nBagType;
        }
    };

    //整理背包返回
    struct SCBagResort : public NetworkMessage
    {
        //返回码(0 - 成功，> 0 失败原因)
        uint8 m_nResult;

        int byteSize()
        {
            return sizeof(m_nResult);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << m_nResult;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> m_nResult;
        }
    };

    /**
    * ● 英雄(卡牌)(0 message)
    */

    /**
    * ● 属性(0 message)
    */

    /**
    * ● 聊天(0 message)
    */

    /**
    * ● 任务(0 message)
    */

    /**
    * ● 战斗(0 message)
    */

    /**
    * ● 房间(0 message)
    */

    /**
    * ● 道具(0 message)
    */

    /**
    * ● 商店(0 message)
    */

    /**
    * ● 排行榜(0 message)
    */

    /**
    * ● 公告(0 message)
    */

    /**
    * ● 称号(0 message)
    */

    /**
    * ● VIP(0 message)
    */

    /**
    * ● test(2 message)
    */

    struct CSTestPacketReq : public NetworkMessage
    {
        uint32 uint_value;
        string string_value;

        int byteSize()
        {
            return sizeof(uint_value) + (string_value.length() + 2);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << uint_value;
            w << string_value;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> uint_value;
            r >> string_value;
        }
    };

    struct SCTestPacketRsp : public NetworkMessage
    {
        uint32 srv_uint_value;
        string srv_string_value;

        int byteSize()
        {
            return sizeof(srv_uint_value) + (srv_string_value.length() + 2);
        }

        void encode(char* buff, int size)
        {
            StreamWriter w(buff, size);
            w << srv_uint_value;
            w << srv_string_value;
        }

        void decode(const char* buff, int size)
        {
            StreamReader r(buff, size);
            r >> srv_uint_value;
            r >> srv_string_value;
        }
    };

}

#endif    //__PROTOCOL_H__