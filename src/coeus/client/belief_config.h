/* -------------------------------------------------------------------------
//  File        :   belief_config.h
//  Author      :   
//  Datet       :   2014/3/2 16:41
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __BELIEF_CONFIG_H_
#define __BELIEF_CONFIG_H_

#include "3rdparty/boost/serialization/singleton.hpp"
#include "game_common/config/configuration.h"
#include "game_common/config/config_manager.h"
#include "config_defind.h"

struct GameBeliefConfigOb
{
    CString csName;
    CString csDecs;
    CString csPicSkin;
};

typedef std::vector<GameBeliefConfigOb> GameBelieObVec;

class BeliefConfig
    : public Configuration
    , public boost::serialization::singleton<BeliefConfig>
{
public:
    GameBelieObVec vecObRole;

public:
    bool parse();
};

#endif // __BELIEF_CONFIG_H_