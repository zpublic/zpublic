#pragma once

class ZLSocketInitSocket
{
public:
    ZLSocketInitSocket(LPWSADATA lpWSAData = NULL, BYTE minorVersion = 2, BYTE majorVersion = 2)
    {
        LPWSADATA lpTemp = lpWSAData;
        if(!lpTemp)
        {
            lpTemp = (LPWSADATA)_alloca(sizeof(WSADATA));
        }

        m_iResult = ::WSAStartup(MAKEWORD(minorVersion, majorVersion), lpTemp);
    }

    ~ZLSocketInitSocket()
    {
        if(IsValid())
        {
            ::WSACleanup();
        }
    }

    int GetResult()
    {
        return m_iResult;
    }
    BOOL IsValid()
    {
        return m_iResult == 0;
    }

private:
    int m_iResult;
};
