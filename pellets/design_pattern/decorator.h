#pragma once


///> 基本组件
class DecoratorComponent
{
public:
    virtual ~DecoratorComponent()
    {

    }

    virtual void Operation(void* lpData = 0) = 0;
};

///> 具体组件
class ConcreteComponent : public DecoratorComponent
{
public:
    virtual ~ConcreteComponent()
    {

    }
};

///> 装饰组件
class Decorator : public DecoratorComponent
{
public:
    Decorator(DecoratorComponent* pComponent = 0) : m_pComponent(pComponent)
    {

    }
    virtual ~Decorator()
    {

    }

    void Decorate(DecoratorComponent* pComponent)
    {
        m_pComponent = pComponent;
    }

protected:
    DecoratorComponent* m_pComponent;
};
