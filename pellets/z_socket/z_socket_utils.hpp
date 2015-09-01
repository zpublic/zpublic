#pragma once

class ZLSocketUtils
{
public:
    static ZLConnectId GenerateConnectionID()
    {
        static volatile ZLConnectId ms_connId = 0;
        ZLConnectId connId = ::InterlockedIncrement((volatile LONG *)&ms_connId);
        return connId;
    }

    static BOOL IsIpAddress(LPCWSTR lpAddress)
    {
        std::wstring sAddress(lpAddress);
        std::vector<std::wstring> vecNum;
        zl::WinUtils::ZLSplitStrW::Split(sAddress, L".", vecNum);
        if (vecNum.size() == 4)
        {
            for (std::vector<std::wstring>::const_iterator it = vecNum.begin();
                it != vecNum.end();
                ++it)
            {
                int i = _wtoi(it->c_str());
                if (i < 0 || i > 255)
                {
                    return FALSE;
                }
            }
            return TRUE;
        }
        return FALSE;
    }

    static BOOL GetIpAddress(LPCWSTR lpAddress, CStringW& sIp)
    {
        if (!lpAddress)
        {
            return FALSE;
        }
        sIp = lpAddress;
        if (!IsIpAddress(lpAddress))
        {
            hostent* host = ::gethostbyname(ZLW2A(lpAddress));
            if (!host)
            {
                return FALSE;
            }
            sIp = inet_ntoa(*(struct in_addr*)(*host->h_addr_list));
        }
        return TRUE;
    }

    static BOOL GetSocketAddress(SOCKET socket, CStringW& sAddress, USHORT& uPort, BOOL bLocal)
    {
        sockaddr addr;
        int addr_len = sizeof(addr);
        int ret = bLocal ? getsockname(socket, &addr, &addr_len) : getpeername(socket, &addr, &addr_len);
        if (ret == NO_ERROR)
        {
            sockaddr_in& addr_in = (sockaddr_in&)addr;
            uPort = ntohs(addr_in.sin_port);
            sAddress = ZLA2W(inet_ntoa(addr_in.sin_addr));
            return sAddress.IsEmpty() ? FALSE : TRUE;
        }
        return FALSE;
    }
};
