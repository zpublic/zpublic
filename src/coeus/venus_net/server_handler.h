#ifndef __SERVER_HANDLER_H__
#define __SERVER_HANDLER_H__

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketReactor.h"

class ServerHandler
{
public:
	ServerHandler(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor):
		_socket(socket),
		_reactor(reactor),
		_buffer(new unsigned char[1024 * 4])
	{
	}

private:
	Poco::Net::SocketReactor& _reactor;
	Poco::Net::StreamSocket   _socket;
	unsigned char*            _buffer;
};

#endif // !__SERVER_HANDLER_H__
