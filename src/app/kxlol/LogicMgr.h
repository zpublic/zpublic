#pragma once
#include "LogicBuild.h"
#include "LogicMake.h"
#include "LogicPractice.h"

class CLogicMgr
{
public:
    CLogicMgr();
    ~CLogicMgr();

    CLogicBuild& Build()
    {
        return m_cBuild;
    }

    CLogicMake& Make()
    {
        return m_cMake;
    }

    CLogicPractice& Practice()
    {
        return m_cPractice;
    }

private:
    CLogicBuild             m_cBuild;
    CLogicMake              m_cMake;
    CLogicPractice          m_cPractice;
};

