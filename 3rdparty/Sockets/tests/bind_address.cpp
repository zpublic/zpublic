#include <Sockets/SocketHandler.h>
#include <Sockets/TcpSocket.h>
#include <Sockets/ListenSocket.h>
#include <Sockets/Exception.h>
#include <Sockets/Utility.h>
#include <iostream>

#ifdef SOCKETS_NAMESPACE
using namespace SOCKETS_NAMESPACE;
#endif

class lSocket : public TcpSocket
{
public:
  lSocket(ISocketHandler& h) : TcpSocket(h) {
    SetLineProtocol();
  }
  void OnAccept() {
    Send("Welcome and goodbye: " + GetRemoteAddress() + ":" + Utility::l2string(GetRemotePort()) + "\n");
    SetCloseAndDelete();
  }
  void OnLine(const std::string& line) {
    std::cout << line << std::endl;
  }
  void OnDisconnect() {
    std::cout << std::endl << "Disconnected" << std::endl;
  }
};

int main(int argc, char *argv[])
{
  try
  {
    SocketHandler h;
    ListenSocket<lSocket> l(h);
    l.Bind( 12343 );
    h.Add(&l);
    time_t t = time(NULL);
    bool connected = false;
    while (true)
    {
      h.Select(1, 0);
      if (time(NULL) > t + 3 && !connected)
      {
        std::cout << std::endl << "Connecting...";
        TcpSocket *p = new lSocket(h);
        Ipv4Address source_address( "localhost", 10000 );
        Ipv4Address dest_address( "localhost", 12343 );
        p -> SetDeleteByHandler();
        p -> Open(dest_address, source_address);
        h.Add(p);
        connected = true;
      }
      std::cout << ".";
      fflush(stdout);
    }
  }
  catch (const Exception& e)
  {
    std::cerr << e.ToString() << std::endl;
  }
}
