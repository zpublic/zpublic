#ifndef __PREPARE_STATEMENT_MANAGER_H__
#define __PREPARE_STATEMENT_MANAGER_H__

#include "venus_net/venus_net.h"
#include "db_define.h"
#include "Poco/Data/Session.h"
#include <venus_net/singleton.h>

enum ExecutorFlag 
{
    EXECUTOR_SYNC,
    EXECUTOR_ASYNC 
};

class PreparedStatement
{
public:
    PreparedStatement(Poco::Data::Session& session, const std::string& sql, ExecutorFlag flag)
        : _session(session), _statement(session), _sql(sql), _flag(flag)
    {
    }

    PreparedStatement& operator = (const PreparedStatement& other)
    {
        this->_sql = other._sql;
        this->_flag = other._flag;
        this->_session = other._session;
        this->_statement = other._statement;

        return *this;
    }

    inline Poco::Data::Statement& statement() { return _statement; }
    inline Poco::Data::Session& session() { return _session; }
    inline const std::string& sql() const { return _sql; }
    uint32 execute()
    {
        debug_log("Excuting SQL statement : %s", _statement.toString().c_str());

        // execute the statement
        uint32 result = _statement.execute();

        // By AngryPowman:
        // Create new statement with current sql (equal of reset() function)
        // reset() allowing to reuse statement with another SQL, we use original sql string to create the statement
        _statement.swap(Poco::Data::Statement(_session.createStatementImpl()));
        _statement << _sql;

        debug_log("Excuted SQL statement, result = %d", result);

        return result;
    }

    bool done()
    {
        return _statement.done();
    }

private:
    std::string _sql;
    ExecutorFlag _flag;
    Poco::Data::Session& _session;
    Poco::Data::Statement _statement;
};

typedef std::vector<PreparedStatement> PreparedStatementList;
class IPrepareStatementManager
{
public:
    IPrepareStatementManager(Poco::Data::Session& session)
        : _session(session)
    {
        _preparedStatementTable.resize(STMT_MAX_FLAG, PreparedStatement(session, "", EXECUTOR_SYNC));
    }

    virtual ~IPrepareStatementManager() {}

    static PreparedStatement& getPreparedStatement(uint16 stmt_index)
    {
        return _preparedStatementTable[stmt_index];
    }

    virtual void doPrepareStatement() = 0;

protected:
    //register prepare statement
    void PREPARE_STATEMENT(uint16 stmt_index, const std::string& sql, ExecutorFlag flag)
    {
        _preparedStatementTable[stmt_index] = PreparedStatement(_session, sql, flag);
        _preparedStatementTable[stmt_index].statement() << sql;
    }

private:
    Poco::Data::Session& _session;
    static PreparedStatementList _preparedStatementTable;
};

class PrepareStatementManager
    : public IPrepareStatementManager
{
public:
    PrepareStatementManager(Poco::Data::Session& session)
        : IPrepareStatementManager(session)
    {
        doPrepareStatement();
    }

    virtual ~PrepareStatementManager() {}
    virtual void doPrepareStatement();
};

#endif // !__PREPARE_STATEMENT_MANAGER_H__
