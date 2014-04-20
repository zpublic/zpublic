#pragma once

#include "def.h"
#include "design_pattern/observer.h"
#include "design_pattern/strategy.h"
#include "design_pattern/decorator.h"
#include "design_pattern/factory.h"
#include "design_pattern/singleton.h"
#include "design_pattern/command.h"


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

///> Command 命令模式
class CommandAdd1 : public CommandBase
{
public:
    virtual void* Execute(void* lpData)
    {
        gDesignPatternTestNum += 1;
        return 0;
    }
};
class CommandAdd10 : public CommandBase
{
public:
    virtual void* Execute(void* lpData)
    {
        gDesignPatternTestNum += 10;
        return 0;
    }
};

///> Adapter 适配器模式
class Target
{
public:
    virtual int Request() = 0;
};
class Adaptee
{
public:
    int SpecificRequest()
    {
        return 1;
    }
};
class ClassAdapter
    : public Target
    , public Adaptee
{
public:
    virtual int Request()
    {
        return SpecificRequest();
    }
};
class ObjectAdapter2 : public Target
{
public:
    virtual int Request()
    {
        return m.SpecificRequest();
    }
private:
    Adaptee m;
};

///> Facade 外观模式
class SubSystem1
{
public:
    void a() {}
};
class SubSystem2
{
public:
    void b() {}
};
class Facade
{
public:
    void Run()
    {
        s1.a();
        s2.b();
    }
private:
    SubSystem1 s1;
    SubSystem2 s2;
};


///> TemplateMethod 模板方法模式
class AbstractClass
{
public:
    virtual ~AbstractClass() {}
    void TemplateMethod()
    {
        PrimitiveOperation1();
        PrimitiveOperation2();
    }
    virtual void PrimitiveOperation1()=0;
    virtual void PrimitiveOperation2()=0;

protected:
    AbstractClass() {}
};
class ConcreteClass1 : public AbstractClass
{
public:
    virtual void PrimitiveOperation1()
    {
        gDesignPatternTestNum = 0;
    }
    virtual void PrimitiveOperation2()
    {
        gDesignPatternTestNum++;
    }
};
class ConcreteClass2 : public AbstractClass
{
public:
    virtual void PrimitiveOperation1()
    {
        gDesignPatternTestNum = 0;
    }
    virtual void PrimitiveOperation2()
    {
        gDesignPatternTestNum += 2;
    }
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

    void testCommand()
    {
        CommandInvoker invoker;
        CommandAdd1 c1;
        CommandAdd10 c10;
        gDesignPatternTestNum = 0;
        invoker.SetCommand(&c1);
        invoker.Invoke();
        TEST_ASSERT(gDesignPatternTestNum == 1);
        invoker.SetCommand(&c10);
        invoker.Invoke();
        TEST_ASSERT(gDesignPatternTestNum == 11);
        invoker.Invoke();
        TEST_ASSERT(gDesignPatternTestNum == 21);
        EmptyCommand cc1;
        invoker.SetCommand(&cc1);
        invoker.Invoke();
        TEST_ASSERT(gDesignPatternTestNum == 21);
        MacroCommand cc2;
        cc2.AddCommand(&c1);
        cc2.AddCommand(&c1);
        cc2.AddCommand(&c10);
        invoker.SetCommand(&cc2);
        invoker.Invoke();
        TEST_ASSERT(gDesignPatternTestNum == 33);
    }

    void testAdapter()
    {
        ClassAdapter a1;
        ObjectAdapter2 a2;
        Target* pt = static_cast<Target*>(&a1);
        TEST_ASSERT(pt->Request() == 1);
        pt = static_cast<Target*>(&a2);
        TEST_ASSERT(pt->Request() == 1);
    }

    void testFacade()
    {
        Facade f;
        f.Run();
    }

    void testTemplateMethod()
    {
        AbstractClass* p = new ConcreteClass1;
        p->TemplateMethod();
        delete p;
        TEST_ASSERT(gDesignPatternTestNum == 1);
        p = new ConcreteClass2;
        p->TemplateMethod();
        delete p;
        TEST_ASSERT(gDesignPatternTestNum == 2);
    }

};