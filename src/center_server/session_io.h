#ifndef __SESSION_IO_H__
#define __SESSION_IO_H__

#include <common.h>

class GameSession;
class NetworkMessage;

typedef std::function<void (GameSession* session)> SessionCreated;
typedef std::function<void (GameSession* session, NetworkMessage* message)> SessionMessage;
typedef std::function<void (GameSession* session)> SessionClosing;

class IODataEventHandler
{
public:
    virtual ~IODataEventHandler() {}

    virtual void onSessionCreated(GameSession* session) = 0;
    virtual void onSessionMessage(GameSession* session, NetworkMessage* message) = 0;
    virtual void onSessionClosing(GameSession* session) = 0;
};

class IODataDispatcher
{
public:
    SessionCreated sessionCreatedEvent;
    SessionMessage sessionMessageEvent;
    SessionClosing sessionClosingEvent;
};

#endif