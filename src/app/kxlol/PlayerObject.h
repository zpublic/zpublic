#pragma once
#include "PlayerBasicInfo.h"
#include "PlayerResBag.h"
#include "PlayerResExt.h"

///> 游戏玩家对象类
class CPlayerObject
{
public:
    CPlayerObject();
    ~CPlayerObject();

    CPlayerBasicInfo& BasicInfo()
    {
        return m_cBasicInfo;
    }

    CPlayerResBag& RegBag()
    {
        return m_cResBag;
    }

    CPlayerResExt& ResExt()
    {
        return m_cResExt;
    }

private:
    CPlayerBasicInfo        m_cBasicInfo;
    CPlayerResBag           m_cResBag;
    CPlayerResExt           m_cResExt;
};
