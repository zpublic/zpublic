#pragma once

struct ParticleInitInfo
{
    DWORD       dwSize;

    float       m_fxPosMin;
    float       m_fxPosMax;
    float       m_fyPosMin;
    float       m_fyPosMax;

    float       m_fLifeMin;
    float       m_fLifeMax;

    float       m_fSpeedBegin;
    float       m_fFullSpeedMin;
    float       m_fFullSpeedMax;

    float       m_fAngle;
};
