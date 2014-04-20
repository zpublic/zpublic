#pragma once

#include "def.h"
#include "design_pattern/observer.h"
#include "design_pattern/strategy.h"
#include "design_pattern/decorator.h"
#include "design_pattern/factory.h"
#include "design_pattern/singleton.h"


extern int gDesignPatternTestNum;

///> Observer 观察者模式
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

///> Strategy 策略模式
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

///> Decorator 装饰模式
class ConcreteComponentX : public ConcreteComponent
{
public:
    virtual void Operation(void* lpData)
    {
        *(int*)lpData = 1;
    }
};
class Decorator1 : public Decorator
{
public:
    virtual void Operation(void* lpData)
    {
        m_pComponent->Operation(lpData);
        *(int*)lpData += 1;
    }
};
class Decorator2 : public Decorator
{
public:
    virtual void Operation(void* lpData)
    {
        m_pComponent->Operation(lpData);
        *(int*)lpData += 2;
    }
};

///> SimpleFactory 简单工厂
class Product
{
public:
    virtual ~Product() {}
    virtual int GetX() = 0;
};
class Product1 : public Product
{
    virtual int GetX() {return 100;}
};
class Product2 : public Product
{
    virtual int GetX() {return 200;}
};

///> Singleton 单例模式
class SingletonXX
{
public:
    int Out()
    {
        return 1;
    }
};
typedef Singleton<SingletonXX> SingletonX;

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

    void testDecorator()
    {
        ConcreteComponentX x;
        Decorator1 d1;
        Decorator2 d2;
        int n = 0;
        x.Operation((void*)&n);
        TEST_ASSERT(n == 1);
        d1.Decorate(static_cast<DecoratorComponent*>(&x));
        d1.Operation((void*)&n);
        TEST_ASSERT(n == 2);
        d2.Decorate(static_cast<DecoratorComponent*>(&x));
        d2.Operation((void*)&n);
        TEST_ASSERT(n == 3);
        d2.Decorate(static_cast<DecoratorComponent*>(&d1));
        d2.Operation((void*)&n);
        TEST_ASSERT(n == 4);
    }

    void testSimpleFactory()
    {
        SimpleFactory<Product> f;
        SimpleFactory<Product>::fnCreateObject fn = CreateObject<Product, Product1>;
        f.AddCreator(1, fn);
        f.AddCreator(2, CreateObject<Product, Product2>);
        Product* p1 = f.CreateObject(1);
        TEST_ASSERT(p1->GetX() == 100);
        delete p1;
        Product* p2 = f.CreateObject(2);
        TEST_ASSERT(p2->GetX() == 200);
        delete p2;
        Product* p3 = f.CreateObject(3);
        TEST_ASSERT(p3 == NULL);

        SimpleFactory<Product, std::string> f2;
        SimpleFactory<Product, std::string>::fnCreateObject fn2 = CreateObject<Product, Product1>;
        f2.AddCreator("a", fn);
        f2.AddCreator("b", CreateObject<Product, Product2>);
        Product* p11 = f2.CreateObject("a");
        TEST_ASSERT(p11->GetX() == 100);
        delete p11;
        Product* p22 = f2.CreateObject("b");
        TEST_ASSERT(p22->GetX() == 200);
        delete p22;
        Product* p33 = f2.CreateObject("c");
        TEST_ASSERT(p33 == NULL);
    }

    void testSingleton()
    {
        // SingletonX x; err
        TEST_ASSERT(SingletonX::Instance().Out() == 1)
    }

};