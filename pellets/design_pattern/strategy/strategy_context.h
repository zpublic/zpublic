#pragma once
#include "strategy.h"

class StrategyContext
{
public:
    StrategyContext(Strategy* pStrategy = 0) : m_pStrategy(pStrategy)
    {

    }
    virtual ~StrategyContext()
    {

    }

    void SetStrategy(Strategy* pStrategy)
    {
        m_pStrategy = pStrategy;
    }

    void* Operate(void* lpData)
    {
        if (m_pStrategy)
        {
            return m_pStrategy->Operate(this, lpData);
        }
        return 0;
    }

protected:
    Strategy* m_pStrategy;
};