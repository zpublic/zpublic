#include "prepare_statement_manager.h"

PreparedStatementList IPrepareStatementManager::_preparedStatementTable;

void PrepareStatementManager::doPrepareStatement()
{
    PREPARE_STATEMENT(STMT_USER_EXISTS,          "SELECT username FROM users WHERE username = ?;", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_USER_AUTH,            "SELECT username, password FROM users WHERE username = ? and password = ?;", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_INSERT_NEW_USER,      "INSERT INTO users(user_guid, username, password, register_ip, register_time) VALUES (?, ?, ?, ?, ?);", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_HAS_CHARACTER,        "SELECT user_guid FROM player_characters WHERE user_guid = ?", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_NICKNAME_IN_USE,      "SELECT nickname FROM player_characters WHERE nickname = ?", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_LOAD_CHARACTER,       "SELECT character_id, character_type, nickname, gender, belief FROM player_characters WHERE userGuid = ?;", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_INSERT_NEW_CHARACTER, "INSERT INTO player_characters(user_guid, character_id, character_type, nickname, gender, belief)"
        " VALUES (?, ?, ?, ?, ?, ?);", EXECUTOR_SYNC);
    PREPARE_STATEMENT(STMT_SAVE_CHARACTER,       "REPLACE INTO player_characters(user_guid, character_id, character_type, nickname, gender, belief)"
        " VALUES (?, ?, ?, ?, ?, ?);", EXECUTOR_SYNC);
}