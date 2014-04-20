#pragma once
#include <list>

class CommandBase
{
public:
    virtual ~CommandBase()
    {

    }

    virtual void* Execute(void* lpData) = 0;
};

class EmptyCommand : public CommandBase
{
public:
    virtual void* Execute(void* lpData)
    {
        return 0;
    }
};

class MacroCommand : public CommandBase
{
public:
    virtual void* Execute(void* lpData)
    {
        std::list<CommandBase*>::iterator it = m_Cmds.begin();
        std::list<CommandBase*>::iterator itEnd = m_Cmds.end();
        for (; it != itEnd; ++it)
        {
            (*it)->Execute(lpData);
        }
        return 0;
    }
    void AddCommand(CommandBase* pCmd)
    {
        m_Cmds.push_back(pCmd);
    }

protected:
    std::list<CommandBase*> m_Cmds;
};

class CommandInvoker
{
public:
    CommandInvoker(CommandBase* pCmd = 0) : m_pCmd(pCmd)
    {

    }
    virtual ~CommandInvoker()
    {

    }

    void SetCommand(CommandBase* pCmd)
    {
        m_pCmd = pCmd;
    }
    void* Invoke(void* lpData = 0)
    {
        if (m_pCmd)
        {
            return m_pCmd->Execute(lpData);
        }
        return 0;
    }

protected:
    CommandBase* m_pCmd;
};
