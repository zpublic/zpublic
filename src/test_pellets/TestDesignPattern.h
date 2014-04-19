#pragma once

#include "def.h"
#include "design_pattern/observer/observable.h"

///> observer 观察者模式

extern int gObserverNum;
class Observer1 : public Observer
{
public:
    virtual void update( Observable* p, void* lpData )
    {
        gObserverNum += (int)lpData;
    }
};
class Observer2 : public Observer
{
public:
    virtual void update( Observable* p, void* lpData )
    {
        gObserverNum += (int)lpData + 1;
    }
};
class ObservableX : public Observable
{

};

class CTestDesignPattern : public Suite
{
public:
    CTestDesignPattern(void);
    ~CTestDesignPattern(void);

    void testObserver()
    {
        gObserverNum = 0;
        Observer1 o1;
        Observer2 o2;
        ObservableX x;
        x.addObserver(static_cast<Observer*>(&o1));
        x.addObserver(static_cast<Observer*>(&o2));
        x.setChanged();
        x.notifyObservers((void*)1);
        TEST_ASSERT(gObserverNum = 3);
        x.notifyObservers((void*)2);
        TEST_ASSERT(gObserverNum = 3);
        x.setChanged();
        x.notifyObservers((void*)2);
        TEST_ASSERT(gObserverNum = 8);
        x.deleteObserver(static_cast<Observer*>(&o2));
        x.setChanged();
        x.notifyObservers((void*)2);
        TEST_ASSERT(gObserverNum = 10);
    }
};