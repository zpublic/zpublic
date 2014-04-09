#ifndef __DB_DEFINE_H__
#define __DB_DEFINE_H__

enum STMT_INDEX
{
    //====================================================================
    // 用户鉴权
    //====================================================================
    STMT_USER_EXISTS,
    STMT_USER_AUTH,
    STMT_INSERT_NEW_USER,

    //====================================================================
    // 角色相关
    //====================================================================
    STMT_HAS_CHARACTER,
    STMT_NICKNAME_IN_USE,
    STMT_LOAD_CHARACTER,
    STMT_SAVE_CHARACTER,
    STMT_INSERT_NEW_CHARACTER,


    STMT_MAX_FLAG
};

#endif // !__DB_DEFINE_H__
