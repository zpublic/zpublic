//////////////////////////////////////////////////////////////////////////
//  Class Name: BKPngPool
// Description: PNG Pool
//     Creator: zhouyibo
//     Version: 2011.6.20 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")

#include <atlcoll.h>
#include "bkresutil.h"

class BkJpgPool
{
public:
    BkJpgPool()
    {
    }
    virtual ~BkJpgPool()
    {
        _Clear();
       
    }

    typedef CAtlMap<UINT, Gdiplus::Image *> _TypeJpgPool;

    static Gdiplus::Image* Get(UINT uResID)
    {
        _TypeJpgPool::CPair* pPairRet = _Instance()->m_mapJpg.Lookup(uResID);
        Gdiplus::Image* pImg = NULL;

        if (NULL == pPairRet)
        {
            _LoadJPGImageFromResourceID(uResID, pImg);
            if (pImg)
                _Instance()->m_mapJpg[uResID] = pImg;
        }
        else
            pImg = pPairRet->m_value;

        return pImg;
    }

    static void Reload()
    {
        _Instance()->_Clear();
    }

    static size_t GetCount()
    {
        return _Instance()->m_mapJpg.GetCount();
    }

protected:
    _TypeJpgPool m_mapJpg;

    static BkJpgPool* ms_pInstance;

    static BkJpgPool* _Instance()
    {
        if (!ms_pInstance)
            ms_pInstance = new BkJpgPool;
        return ms_pInstance;
    }

    static BOOL _LoadJPGImageFromResourceID(UINT nID, Gdiplus::Image* &pImg)
    {
		CStringA strResource;
		IStream* pStm = NULL;

		BOOL bRet = BkResManager::LoadResource(nID, strResource, BKRES_JPG_TYPE, TRUE);
		int len = strResource.GetLength();

		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, len);
		BYTE* pMem = (BYTE*)::GlobalLock(hMem);

		memcpy(pMem, (LPCSTR)strResource, len);
	
		if ( CreateStreamOnHGlobal( hMem, TRUE, &pStm ) !=S_OK ) 	
			return FALSE;   

		pImg = Gdiplus::Image::FromStream(pStm);

		::GlobalUnlock(hMem);
		pStm->Release();

		return TRUE;

    }

    void _Clear()
    {
        Gdiplus::Image *pImg = NULL;

        POSITION pos = m_mapJpg.GetStartPosition();

        while (pos != NULL) 
        {
            pImg = m_mapJpg.GetNextValue(pos);
            if (pImg)
                delete pImg;
        }

        m_mapJpg.RemoveAll();
    }
};

__declspec(selectany) BkJpgPool* BkJpgPool::ms_pInstance = NULL;