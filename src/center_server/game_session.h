#ifndef __GAME_SESSION_H__
#define __GAME_SESSION_H__

#include "network_session.h"
#include <Poco/Util/Timer.h>

class Player;
class Poco::Util::Timer;
class GameSession
    : public NetworkSession
{
	struct SessionHeartbeat
	{
		// 心跳检查规则：
		// 假设正常报心跳时间是40秒一个周期，将允许前后误差3秒，也就是说在37~43秒之间报都是合法的。
		// 如果不在范围内报心跳，则记为一次异常，超过指定次数后踢掉连接。

		static const int32 HEARTBEAT_TIME  = 10000;    //心跳报时时间（毫秒）
		static const int32 DEVIATION_VALUE = 3000;     //允许误差值（毫秒）
        static const int32 CHECK_PERIOD    = 12000;    //检查周期（毫秒），必须大于或等于心跳周期
        static const int32 FAILED_COUNT    = 3;        //到达该失败次数后踢掉连接

		static const int32 DOWN_DEVIATION_VALUE = HEARTBEAT_TIME - DEVIATION_VALUE;
		static const int32 UP_DEVIATION_VALUE = HEARTBEAT_TIME + DEVIATION_VALUE;

		SessionHeartbeat() 
		{
			cleanup();
		}

		void cleanup()
		{
			last_heartbeat_time = 0;
			failed_count = 0;
		}

		int64 last_heartbeat_time;
		int32 failed_count;
	};

public:
    GameSession(const uint64& session_id);
    virtual ~GameSession();

    bool init();
    void destroy();

public:
    Player* getPlayer();

private:
    void attackPlayerPtr(Player* player);

public:
	//心跳
	void heartbeat_handler(const NetworkMessage& message);

    //登录模块
    void user_login_handler(const NetworkMessage& message);
    void user_register_handler(const NetworkMessage& message);

    //玩家资料模块
    void get_player_profile_handler(const NetworkMessage& message);

    //聊天模块
    void chat_message_handler(const NetworkMessage& message);

    //房间模块
    void room_create_handler(const NetworkMessage& message);
    void get_room_list_handler(const NetworkMessage& message);
    void broadcast_room_add(uint32 id, const std::string& roomName, bool isPublic);
    void enter_room_handler(const NetworkMessage& message);

private:
    void startHeartbeatCheck(long interval = SessionHeartbeat::CHECK_PERIOD);
    void stopHeartbeatCheck();
    void onHeartbeatCheck(Poco::Util::TimerTask& task);
	void heartbeatFailed();

private:
    Player* _player;
	SessionHeartbeat _heartbeat;
    Poco::Util::Timer* _heartbeat_checker;
};

#endif