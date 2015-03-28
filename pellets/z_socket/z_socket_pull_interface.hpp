#pragma once

///> PULL 模型组件接口
class IZLPullClient
{
public:
    /**
    * @brief     抓取数据
    * @param[in] pBuffer                数据抓取缓冲区
    * @param[in] iLength                抓取数据长度
    * @return    TRUE 成功 FALSE 失败
    */
    virtual BOOL Fetch(BYTE* pBuffer, int iLength) = 0;

    /**
    * @brief     窥探数据（不会移除缓冲区数据）
    * @param[in] pBuffer                数据抓取缓冲区
    * @param[in] iLength                抓取数据长度
    * @return    TRUE 成功 FALSE 失败
    */
    virtual BOOL Peek(BYTE* pBuffer, int iLength) = 0;

public:
    virtual ~IZLPullClient() {}
};
