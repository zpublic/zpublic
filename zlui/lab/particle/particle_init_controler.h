#pragma once

class ParticleInitControler_Fixed
{
public:
    ParticleInitControler_Fixed(ParticleItem item)
    {
        memcpy(&m_item, &m_item, sizeof(item));
    }
    void SetInfo(ParticleItem item)
    {
        memcpy(&m_item, &m_item, sizeof(item));
    }

    virtual ~ParticleInitControler_Fixed() {}

private:
    ParticleItem    m_item;
};

class ParticleInitControler_NomalRandom
{
public:
    ParticleInitControler_NomalRandom(ParticleInitInfo info)
    {
        assert(info.dwSize = sizeof(info));
        if (info.dwSize = sizeof(info))
        {
            memcpy(&m_info, &info, sizeof(info));
        }
    }
    void SetInfo(ParticleInitInfo info)
    {
        assert(info.dwSize = sizeof(info));
        if (info.dwSize = sizeof(info))
        {
                memcpy(&m_info, &info, sizeof(info));
        }
    }

    virtual ~ParticleInitControler_NomalRandom() {}

    void InitItem(ParticleItem* pItem)
    {
        pItem->m_fLifeLive      = 0.0f;
        pItem->m_fAngle         = m_info.m_fAngle;
        pItem->m_fSpeed         = m_info.m_fSpeedBegin;
        pItem->m_fFullSpeed     = Random_Float(m_info.m_fFullSpeedMin, m_info.m_fFullSpeedMax);
        pItem->m_fxPos          = Random_Float(m_info.m_fxPosMin, m_info.m_fxPosMax);
        pItem->m_fyPos          = Random_Float(m_info.m_fyPosMin, m_info.m_fyPosMax);
        pItem->m_fLife          = Random_Float(m_info.m_fLifeMin, m_info.m_fLifeMax);
    }

protected:
    ParticleInitInfo    m_info;
};