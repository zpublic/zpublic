#pragma once

struct ParticleItem
{
    ///> 当前位置
    float   m_fxPos;
    float   m_fyPos;

    ///> 已存在时间和总生命时长
    float   m_fLifeLive;
    float   m_fLife;

    ///> 最大速度和当前速度
    float   m_fFullSpeed;
    float   m_fSpeed;

    ///> 当前前进的角度
    float   m_fAngle;
};
