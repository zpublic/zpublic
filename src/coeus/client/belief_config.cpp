/* -------------------------------------------------------------------------
//  File        :   belief_config.cpp
//  Author      :   
//  Datet       :   2014/3/2 16:41
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "belief_config.h"
#include "game_common\path_definition.h"

bool BeliefConfig::parse()
{
    WCHAR szFilePath[MAX_PATH + 1] = {0};
    ::GetModuleFileNameW(0, szFilePath, MAX_PATH);
    ::PathRemoveFileSpecW(szFilePath);
    ::PathAppend(szFilePath, CA2W(ConfigFile::BadgeConfigPath.c_str(), CP_UTF8));

    Json::Value value;
    LOAD_CONFIG(std::string(CW2A(szFilePath, CP_UTF8)), value);

    Json::Value jsonRoot = value["belief"];

    if (jsonRoot == Json::nullValue)
    {
        return false;
    }

    CStringA strName;
    CStringA strDesc;
    CStringA strPicSkin;
    int nSize = jsonRoot.size();

    for (int nIndex = 0; nIndex < nSize; ++nIndex)
    {
        GameBeliefConfigOb configOb;
        Json::Value jsonIndexValue = jsonRoot[nIndex];

        if (jsonIndexValue == Json::nullValue)
        {
            continue;
        }

        strName = jsonIndexValue["belief_name"].asCString();
        strDesc = jsonIndexValue["belief_desc"].asCString();
        strPicSkin = jsonIndexValue["belief_pic"].asCString();

        configOb.csName = CA2W(strName, CP_UTF8).m_psz;
        configOb.csDecs = CA2W(strDesc, CP_UTF8).m_psz;
        configOb.csPicSkin = CA2W(strPicSkin, CP_UTF8).m_psz;

        vecObRole.push_back(configOb);
        strName.Empty();
        strDesc.Empty();
        strPicSkin.Empty();
    }

    return true;
}
