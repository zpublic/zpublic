#ifndef __GAME_DATABASE_H__
#define __GAME_DATABASE_H__

#include "venus_net/venus_net.h"
#include "venus_net/singleton.h"
#include "Poco/Data/Common.h"
#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/SessionFactory.h"

class PlayerDB;
class PrepareStatementManager;
class GameDatabase
    : public Venus::Singleton<GameDatabase>
{
public:
    GameDatabase();
    virtual ~GameDatabase();

public:
    bool initialize();
    void destroy();

public:
    //====================================================================
    // 用户系统
    //====================================================================

    //检查数据库中一个用户是否存在
    bool isUserExists(const std::string& username);

    //验证帐号和密码是否匹配
    bool userAuth(const std::string& username, const std::string& password);

    //插入新的用户记录
    void insertNewUserRecord(
        uint64 user_guid, 
        const std::string& username,
        const std::string& password,
        const std::string& register_ip,
        uint64 register_timestamp
        );

    //====================================================================
    // 角色相关
    //====================================================================

    //对应的账户下是否存在角色
    bool hasCharacter(uint64 user_guid);

    //检查昵称是否存在
    bool isNicknameExist(const std::string& nickname);

    //加载角色（全部数据）
    bool loadCharacterInfo(uint64 userGuid, PlayerDB* playerDB);

    //保存角色（全部数据）
    bool saveCharacterInfo(uint64 userGuid, PlayerDB* playerDB);

    //创建角色（基本数据）
    bool createCharacter(uint64 userGuid, uint8 characterType, const std::string& nickname, uint8 gender, uint8 belief);

private:
    Poco::Data::Session* _db_session;
    Poco::Data::Statement* _db_stmt;
    PrepareStatementManager* _prepareStatementManager;
};

#endif