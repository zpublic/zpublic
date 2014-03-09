/* -------------------------------------------------------------------------
//  File        :   res_img_pool.h
//  Author      :   
//  Datet       :   2014/3/2 21:27
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __RES_IMG_POOL_H_
#define __RES_IMG_POOL_H_

#include "3rdparty/boost/serialization/singleton.hpp"
#include "config_defind.h"

class ResImgPool
    : public boost::serialization::singleton<ResImgPool>
{
public:
    ResImgPool();

    ~ResImgPool();

    BOOL Initialize();

    BOOL Unitialize();

    BOOL Load();

    BOOL Load(UINT nId, LPCTSTR lpszPath);

private:
    BOOL _GenerateSkinXML(CStringA& csXML);

private:
    CString m_csPath;
};

#endif // __RES_IMG_POOL_H_