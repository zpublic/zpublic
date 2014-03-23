/* -------------------------------------------------------------------------
//  File        :   role_config.cpp
//  Author      :   
//  Datet       :   2014/3/2 14:08
//  Brief       :   
//
// -----------------------------------------------------------------------*/
//
#include "stdafx.h"
#include "role_config.h"
#include "game_common\path_definition.h"

bool RoleConfig::parse()
{
    Json::Value value;
    LOAD_CLIENT_CONFIG(ConfigFile::RoleConfigPath, value);

    Json::Value jsonRoot = value["characters"];

    if (jsonRoot == Json::nullValue)
    {
        return false;
    }

    CStringA strName;
    CStringA strDesc;
    int nSize = jsonRoot.size();

    for (int nIndex = 0; nIndex < nSize; ++nIndex)
    {
        GameRoleConfigOb configOb;
        Json::Value jsonIndexValue = jsonRoot[nIndex];

        if (jsonIndexValue == Json::nullValue)
        {
            continue;
        }

        strName = jsonIndexValue["char_name"].asCString();
        strDesc = jsonIndexValue["char_desc"].asCString();

        configOb.csName = CA2W(strName, CP_UTF8).m_psz;
        configOb.csDecs = CA2W(strDesc, CP_UTF8).m_psz;

        if (!_ParseRoleImg(jsonIndexValue, configOb))
        {
            continue;
        }

        vecObRole.push_back(configOb);
        strName.Empty();
        strDesc.Empty();
    }

    return true;
}

BOOL RoleConfig::_ParseRoleImg(Json::Value& jsonRoleImg, GameRoleConfigOb& configRoleOb)
{
    if (jsonRoleImg == Json::nullValue)
    {
        return FALSE;
    }

    _ParseJsonList(jsonRoleImg, "char_cg", configRoleOb.vecCgImage);
    _ParseJsonList(jsonRoleImg, "char_avatar", configRoleOb.vecAvatarImage);

    return TRUE;
}

BOOL RoleConfig::_ParseJsonList(Json::Value& jsonRole, const std::string& strName, GameResVec& configVecOb)
{
    if (jsonRole == Json::nullValue
        || strName.empty())
    {
        return FALSE;
    }

    CStringA strValue;
    Json::Value jsonParseValue = jsonRole[strName];

    if (jsonParseValue == Json::nullValue)
    {
        return FALSE;
    }

    int nJsonSize = jsonParseValue.size();
    for (int nIndex = 0; nIndex < nJsonSize; ++nIndex)
    {
        Json::Value jsonParseIndex = jsonParseValue[nIndex];

        if (jsonParseIndex == Json::nullValue)
        {
            continue;
        }

        strValue = jsonParseIndex.asCString();
        configVecOb.push_back(CA2W(strValue).m_psz);
        strValue.Empty();
    }

    return TRUE;
}

BOOL RoleConfig::gainRoleConfigInfo(UINT nRoleId, UINT nImageId, CString& csName, CString& csDecs, CStringA& csSkin)
{
    if (vecObRole.size() < (nRoleId + 1)
        || vecObRole[nRoleId].vecCgImage.size() < (nImageId + 1))
    {
        return FALSE;
    }

    csName = vecObRole[nRoleId].csName;
    csDecs = vecObRole[nRoleId].csDecs;
    csSkin = CW2A(vecObRole[nRoleId].vecCgImage[nImageId]).m_psz;
    return TRUE;
}
