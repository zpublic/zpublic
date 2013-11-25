#include "game_session.h"
#include "game_session_manager.h"
#include "player.h"
#include "player_manager.h"
#include <Poco/Util/TimerTaskAdapter.h>

GameSession::GameSession(const uint64& session_id)
    : NetworkSession(session_id)
{

}

GameSession::~GameSession()
{
}


bool GameSession::init()
{
	_player = nullptr;
	_heartbeat_checker = nullptr;

    return true;
}

void GameSession::destroy()
{
    stopHeartbeatCheck();
	PlayerManager::getInstance().removePlayer(_player);
}

void GameSession::heartbeat_handler(const NetworkMessage& message)
{
	std::time_t now = Poco::Timestamp().epochTime() * 1000;
	int64 interval = now - _heartbeat.last_heartbeat_time;

	if (//interval < SessionHeartbeat::DOWN_DEVIATION_VALUE || 
        interval > SessionHeartbeat::UP_DEVIATION_VALUE)
	{
		heartbeatFailed();
	}
	else
	{
		_heartbeat.failed_count = 0;
		_heartbeat.last_heartbeat_time = now;

        debug_log("[%ull] - > heartbeat successful. failed_count = %d, last_heartbeat_time = %ld", 
            this->_player->guid(),
            _heartbeat.failed_count,
            _heartbeat.last_heartbeat_time
            );
	}
}

Player* GameSession::getPlayer()
{
    return _player;
}

void GameSession::attackPlayerPtr(Player* player)
{
    _player = player;
}

void GameSession::onHeartbeatCheck(Poco::Util::TimerTask& task)
{
    //info_log("[%ull] - > Check Heartbeat, _heartbeat.failed_count = %d", _player->guid(), _heartbeat.failed_count);

    std::time_t now = Poco::Timestamp().epochTime() * 1000;
    int64 interval = now - _heartbeat.last_heartbeat_time;

    if (interval > SessionHeartbeat::UP_DEVIATION_VALUE)
    {
		debug_log("Player %ull's connection maybe lost(server could not recieve heartbeat for a long time), close session.", _player->guid());
		closeSession();
    }
    else
    {
        _heartbeat.failed_count = 0;
    }

    debug_log("[%ull] - > Heartbeat checking. _heartbeat.failed_count = %d", _player->guid(), _heartbeat.failed_count);
}

void GameSession::startHeartbeatCheck(long interval/* = SessionHeartbeat::CHECK_PERIOD*/)
{
    //初始化心跳时间
    _heartbeat.last_heartbeat_time = Poco::Timestamp().epochTime() * 1000;

    //开始心跳检查
	Poco::Util::TimerTask::Ptr task 
		= new Poco::Util::TimerTaskAdapter<GameSession>(*this, &GameSession::onHeartbeatCheck);

	_heartbeat_checker = new Poco::Util::Timer(Poco::Thread::PRIO_NORMAL);

    //登录之后一个心跳周期之后再执行检查
    //  注：检查周期设置为比心跳周期大1~2秒为佳，避免出现服务端刚执行完检查逻辑后才收到包
	_heartbeat_checker->schedule(task, interval, interval);
}

void GameSession::stopHeartbeatCheck()
{
	if (_heartbeat_checker != nullptr)
    {
		_heartbeat_checker->cancel(true);
        SAFE_DELETE(_heartbeat_checker);
    }
}

void GameSession::heartbeatFailed()
{
	_heartbeat.failed_count += 1;
	debug_log("Player %ull unormal heartbeat happened, failed_count = %d", _player->guid(), _heartbeat.failed_count);

	//如果连续三次心跳时间异常，则判定为网络不稳定，踢掉
	if (_heartbeat.failed_count >= SessionHeartbeat::FAILED_COUNT)
	{
		debug_log("Player %ull failed heartbeat at the limit, close session.", _player->guid());
		GameSessionManager::getInstance().destroySession(this);
	}
}
