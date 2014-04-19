#pragma once

#include "def.h"
#include "design_pattern/observer/observable.h"
#include "design_pattern/strategy/strategy_context.h"


extern int gDesignPatternTestNum;

///> observer 观察者模式
class Observer1 : public Observer
{
public:
    virtual void update( Observable* p, void* lpData )
    {
        gDesignPatternTestNum += (int)lpData;
    }
};
class Observer2 : public Observer
{
public:
    virtual void update( Observable* p, void* lpData )
    {
        gDesignPatternTestNum += (int)lpData + 1;
    }
};
class ObservableX : public Observable
{
};

///> strategy 策略模式
class Strategy1 : public Strategy
{
public:
    virtual void* Operate(StrategyContext* p, void* lpData)
    {
        int n = (int)lpData + 1;
        return (void*)n;
    }
};
class Strategy2 : public Strategy
{
public:
    virtual void* Operate(StrategyContext* p, void* lpData)
    {
        int n = (int)lpData + 2;
        return (void*)n;
    }
};
class StrategyContextX : public StrategyContext
{
};

class CTestDesignPattern : public Suite
{
public:
    CTestDesignPattern(void);
    ~CTestDesignPattern(void);

    void testObserver()
    {
        gDesignPatternTestNum = 0;
        Observer1 o1;
        Observer2 o2;
        ObservableX x;
        x.addObserver(static_cast<Observer*>(&o1));
        x.addObserver(static_cast<Observer*>(&o2));
        x.setChanged();
        x.notifyObservers((void*)1);
        TEST_ASSERT(gDesignPatternTestNum = 3);
        x.notifyObservers((void*)2);
        TEST_ASSERT(gDesignPatternTestNum = 3);
        x.setChanged();
        x.notifyObservers((void*)2);
        TEST_ASSERT(gDesignPatternTestNum = 8);
        x.deleteObserver(static_cast<Observer*>(&o2));
        x.setChanged();
        x.notifyObservers((void*)2);
        TEST_ASSERT(gDesignPatternTestNum = 10);
    }

    void testStrategy()
    {
        Strategy1 s1;
        Strategy2 s2;
        StrategyContextX x;
        void* ret = x.Operate(0);
        TEST_ASSERT((int)ret == 0);
        x.SetStrategy(static_cast<Strategy*>(&s1));
        ret = x.Operate(0);
        TEST_ASSERT((int)ret == 1);
        x.SetStrategy(static_cast<Strategy*>(&s2));
        ret = x.Operate(0);
        TEST_ASSERT((int)ret == 2);
    }
};