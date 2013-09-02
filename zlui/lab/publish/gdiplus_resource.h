#pragma once

class GdiPlusResource
{
public:
    static BOOL GetResourceBuff(
        HMODULE hMod,
        LPCWSTR lpName,
        LPCWSTR lpType,
        BYTE** pOutBuf,
        DWORD& dwLen)
    {
        *pOutBuf = NULL;
        BOOL    bRet    = FALSE;
        HRSRC   hRsrc   = NULL;
        HGLOBAL hGlobal = NULL;
        do 
        {
            hRsrc = ::FindResourceW(hMod, lpName, lpType);
            if (NULL == hRsrc)
            {
                break;
            }
            DWORD dwSize = ::SizeofResource (hMod, hRsrc);
            if (0 == dwSize)
            {
                break;
            }
            hGlobal = ::LoadResource(hMod, hRsrc);
            if (NULL == hGlobal)
            {
                break;
            }
            void* pBuff = ::LockResource(hGlobal);
            dwLen = dwSize + 1;
            *pOutBuf = new BYTE[dwLen];
            if (NULL == *pOutBuf)
            {
                break;
            }
            ::ZeroMemory(*pOutBuf, dwLen);
            memcpy(*pOutBuf, pBuff, dwSize);
            bRet = TRUE;
        } while (0);
        if (hGlobal)
        {
            ::FreeResource(hGlobal);
        }
        return bRet;
    }

    static BOOL Buff2Stream(BYTE* pBuff, DWORD dwLen, IStream** ppStream)
    {
        BOOL bRet = FALSE;
        HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, dwLen);
        if (hGlobal)
        {
            void* pData = ::GlobalLock(hGlobal);
            if (pData)
            {
                memcpy(pData, pBuff, dwLen);
            }
            ::GlobalUnlock(hGlobal);
            if (::CreateStreamOnHGlobal(hGlobal, TRUE, ppStream) == S_OK)
            {
                bRet = TRUE;
            }
        }
        return bRet;
    }

    static BOOL Resource2Stream(HMODULE hMod, LPCWSTR lpName, LPCWSTR lpType, IStream** ppStream)
    {
        BYTE* pBuff = NULL;
        DWORD dwLen = 0;
        GetResourceBuff(hMod, lpName, lpType, &pBuff, dwLen);
        if (NULL == pBuff)
        {
            return FALSE;
        }
        BOOL bRet = Buff2Stream(pBuff, dwLen, ppStream);
        if (pBuff)
        {
            delete []pBuff;
        }
        return bRet;
    }
};

Image* LoadSkinImage(DWORD dwResID, LPCWSTR lpType = L"PNG")
{
    CComPtr<IStream> spStream;
    GdiPlusResource::Resource2Stream(GetModuleHandle(NULL), MAKEINTRESOURCEW(dwResID), L"PNG", &spStream);
    if (NULL == spStream)
    {
        return NULL;
    }
    return Image::FromStream(spStream);
}
