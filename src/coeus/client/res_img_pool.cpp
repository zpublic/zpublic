/* -------------------------------------------------------------------------
//  File        :   res_img_pool.cpp
//  Author      :   Sum
//  Datet       :   2014/3/2 21:30
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "res_img_pool.h"
#include "img_config.h"
#include "bkres\bkresutil.h"

#include <gdiplus.h>

ResImgPool::ResImgPool()
{

}

ResImgPool::~ResImgPool()
{

}

BOOL ResImgPool::Initialize()
{
    WCHAR szFilePath[MAX_PATH + 1] = {0};
    ::GetModuleFileNameW(0, szFilePath, MAX_PATH);
    ::PathRemoveFileSpecW(szFilePath);
    m_csPath = szFilePath;
    m_csPath += L"\\";

    return TRUE;
}

BOOL ResImgPool::Unitialize()
{
    return TRUE;
}

BOOL ResImgPool::Load()
{
    //BkSkin::LoadSkins();
    BkResManager::SetResourcePath(m_csPath + ImgConfig::get_mutable_instance().csResPath);
    return TRUE;
}

BOOL ResImgPool::Load(UINT nId, LPCTSTR lpszPath)
{
    return TRUE;
}

BOOL ResImgPool::_GenerateSkinXML(CStringA& csXML)
{
    csXML = "<skins>";
    CStringA strItem;
    GameResMapPool::const_iterator it = ImgConfig::get_mutable_instance().mapResImg.begin();
    GameResMapPool::const_iterator endit = ImgConfig::get_mutable_instance().mapResImg.end();

    for (; it != endit; ++it)
    {
        strItem.Format("<png name=\"%s\" src=\"%d\"/>", it->second, it->first);
        csXML += strItem;
    }
      
    csXML += "</skins>";
    return TRUE;
}
