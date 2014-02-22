#ifndef __GAME_DEFINE_H__
#define __GAME_DEFINE_H__


enum Gender
{
    GENDER_FEMALE,   // 女性
    GENDER_MALE      // 男性
};

enum LoginResult
{
    LR_LOGIN_SUCCESS,       // 登录成功
    LR_ACCOUNT_DISMATCH,    // 用户名和密码不匹配
    LR_USER_BANNED,         // 用户被封号
};

#endif // !__GAME_DEFINE_H__
