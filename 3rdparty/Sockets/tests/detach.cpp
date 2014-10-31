#include <Sockets/TcpSocket.h>
#include <Sockets/SocketHandler.h>
#include <Sockets/ListenSocket.h>
#include <Sockets/Utility.h>
#include <iostream>

#ifdef SOCKETS_NAMESPACE
using namespace SOCKETS_NAMESPACE;
#endif

	int count = 0;

class EchoSocket : public TcpSocket
{
public:
	EchoSocket(ISocketHandler& h) : TcpSocket(h) {
		SetLineProtocol();
	}

	void OnAccept() {
		std::cout << "OnAccept ThreadId#" << Utility::ThreadID() << ", fd " << GetSocket() << std::endl;
		if (!Detach())
			std::cerr << "Detach() failed" << std::endl;
/*
		else
			std::cout << "Detach() OK" << std::endl;
		std::cout << "	IsDetach(): " << (IsDetach() ? "true" : "false") << std::endl;
*/
	}

	void OnDetached() {
		std::cout << "OnDetached" << std::endl;
		++count;
	}

	void OnLine(const std::string& line) {
//		std::cout << "OnLine #" << Utility::ThreadID() << std::endl;
		Send(line + ", count: " + Utility::l2string(count) + "\n");
		if (!line.empty())
		{
			SetCloseAndDelete();
		}
	}

	void OnDelete() {
//		std::cout << "OnDelete" << std::endl;
	}
};

int main(int argc, char *argv[])
{
	SocketHandler h;
	ListenSocket<EchoSocket> l(h);
	l.Bind(10244);
	h.Add(&l);
	while (true)
		h.Select();
}

