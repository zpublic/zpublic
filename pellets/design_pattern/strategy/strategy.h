#pragma once

class StrategyContext;

class Strategy
{
public:
    virtual ~Strategy()
    {

    }

    virtual void* Operate(StrategyContext* p, void* lpData) = 0;
};