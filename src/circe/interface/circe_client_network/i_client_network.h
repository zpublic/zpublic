#pragma once

interface IClentNetworkListener
{
    virtual void _stdcall OnConnect()                               = NULL;
    virtual void _stdcall OnClose()                                 = NULL;
    virtual void _stdcall OnReceive(const BYTE* pData, int iLength) = NULL;
    virtual void _stdcall OnError(int iErrorCode)                   = NULL;
};

[
    uuid("652336a6-cb4c-4f51-a184-2c726a2cbcce")
]
interface IClientNetwork
{
    virtual int _stdcall SetListener(IClentNetworkListener* pListener)      = NULL;
    virtual int _stdcall Start(LPCTSTR pszRemoteAddress, USHORT usPortt)    = NULL;
    virtual int _stdcall Stop()                                             = NULL;
    virtual int _stdcall Send(const BYTE* pBuffer, int iLength)             = NULL;
};
