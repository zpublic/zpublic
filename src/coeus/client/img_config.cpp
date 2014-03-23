/* -------------------------------------------------------------------------
//  File        :   img_config.cpp
//  Author      :   
//  Datet       :   2014/3/2 21:02
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "img_config.h"
#include "game_common\path_definition.h"

bool ImgConfig::parse()
{
    WCHAR szFilePath[MAX_PATH + 1] = {0};
    ::GetModuleFileNameW(0, szFilePath, MAX_PATH);
    ::PathRemoveFileSpecW(szFilePath);
    ::PathAppend(szFilePath, CA2W(ConfigFile::ImgConfigPath.c_str(), CP_UTF8));

    Json::Value value;
    LOAD_CONFIG(std::string(CW2A(szFilePath, CP_UTF8)), value);

    Json::Value jsonRoot = value["resimg"];

    if (jsonRoot == Json::nullValue)
    {
        return false;
    }

    csResPath = CA2W(jsonRoot["res_path"].asCString(), CP_UTF8).m_psz;

//     CStringA strPath;
//     int nImgId = 0;
//     int nSize = jsonRoot.size();
// 
//     for (int nIndex = 0; nIndex < nSize; ++nIndex)
//     {
//         Json::Value jsonIndexValue = jsonRoot[nIndex];
// 
//         if (jsonIndexValue == Json::nullValue)
//         {
//             continue;
//         }
// 
//         nImgId = jsonIndexValue["img_id"].asInt();
//         strPath = jsonIndexValue["img_path"].asCString();
//         mapResImg[nImgId] = CA2W(strPath, CP_UTF8).m_psz;
//         nImgId = 0;
//     }

    return true;
}
