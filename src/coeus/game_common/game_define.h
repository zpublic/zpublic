#ifndef __GAME_DEFINE_H__
#define __GAME_DEFINE_H__

#define DB_TYPE_SQLITE (0x1)
#define DB_TYPE_MYSQL  (0x2)

enum Gender
{
    Female = 0,   // 女性
    Male          // 男性
};

enum Belief
{
    Holy = 0,           // 神圣教廷
    TechHarbinger,      // 科技先驱
    DarkBoundary,       // 黑暗边界
    BeliefMax
};

enum CharacterCareer
{
    MirrorHunter = 0,   // 镜子猎人
    Mechanic,           // 机械师
    CharacterCareerMax
};

enum LoginResult
{
    LR_LOGIN_SUCCESS,       // 登录成功
    LR_ACCOUNT_DISMATCH,    // 用户名和密码不匹配
    LR_USER_BANNED,         // 用户被封号
};

#endif // !__GAME_DEFINE_H__
