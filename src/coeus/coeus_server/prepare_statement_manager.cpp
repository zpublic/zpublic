#include "prepare_statement_manager.h"

PreparedStatementList IPrepareStatementManager::_preparedStatementTable;

void PrepareStatementManager::doPrepareStatement()
{
    PREPARE_STATEMENT(STMT_USER_EXISTS, "SELECT username FROM users WHERE username = ?;", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_USER_AUTH, "SELECT username, password FROM users WHERE username = ? and password = ?;", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_INSERT_NEW_USER,"INSERT INTO users(user_guid, username, password, register_ip, register_time) VALUES (?, ?, ?, ?, ?);", EXECUTOR_SYNC);
}