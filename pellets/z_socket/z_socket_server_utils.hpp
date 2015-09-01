#pragma once

class ZLSocketServerUtils
{
public:
    static void CloseSocketObj(TSocketObj* pObj)
    {
        if (pObj && pObj->socket != INVALID_SOCKET)
        {
            z_mutex_guard g(*(pObj->pmutex));
            if (pObj->socket != INVALID_SOCKET)
            {
                shutdown(pObj->socket, SD_SEND);
                closesocket(pObj->socket);
                pObj->socket = INVALID_SOCKET;
            }
        }
    }

    static PVOID GetExtensionFucPtr(SOCKET socket, GUID guid)
    {
        DWORD dwBytes;
        PVOID pFn = NULL;
        ::WSAIoctl(
            socket,
            SIO_GET_EXTENSION_FUNCTION_POINTER,
            &guid,
            sizeof(guid),
            &pFn,
            sizeof(pFn),
            &dwBytes,
            NULL,
            NULL);
        return pFn;
    }
};
