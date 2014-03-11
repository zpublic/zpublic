/* -------------------------------------------------------------------------
//  File        :   img_config.h
//  Author      :   
//  Datet       :   2014/3/2 21:01
//  Brief       :   
//
// -----------------------------------------------------------------------*/
#ifndef __IMG_CONFIG_H_
#define __IMG_CONFIG_H_

#include "3rdparty/boost/serialization/singleton.hpp"
#include "game_common/config/configuration.h"
#include "game_common/config/config_manager.h"
#include "config_defind.h"

class ImgConfig
    : public Configuration
    , public boost::serialization::singleton<ImgConfig>
{
public:
    GameResMapPool mapResImg;
    CString csResPath;

public:
    bool parse();
};

#endif // __IMG_CONFIG_H_