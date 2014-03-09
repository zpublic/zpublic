/* -------------------------------------------------------------------------
//  File        :   role_config.h
//  Author      :   
//  Datet       :   2014/3/2 13:58
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __RESOURCE_CONFIG_H_
#define __RESOURCE_CONFIG_H_

#include "3rdparty/boost/serialization/singleton.hpp"
#include "game_common/config/configuration.h"
#include "game_common/config/config_manager.h"
#include "config_defind.h"

struct GameRoleConfigOb
{
    CString csName;
    CString csDecs;
    GameResVec vecCgImage;
    GameResVec vecAvatarImage;
};

typedef std::vector<GameRoleConfigOb> GameRoleObVec;

class RoleConfig
    : public Configuration
    , public boost::serialization::singleton<RoleConfig>
{
public:
    GameRoleObVec vecObRole;

public:
    bool parse();

    BOOL gainRoleConfigInfo(UINT nRoleId,
        UINT nImageId,
        CString& csName,
        CString& csDecs,
        CStringA& csSkin);

private:
    BOOL _ParseRoleImg(Json::Value& jsonRoleImg, GameRoleConfigOb& configRoleOb);
    BOOL _ParseJsonList(Json::Value& jsonRole, const std::string& strName, GameResVec& configVecOb);
};

#endif // __RESOURCE_CONFIG_H_