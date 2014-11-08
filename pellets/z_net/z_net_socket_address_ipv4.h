// #pragma once
// #include "z_net_socket_address.h"
// 
// class ZLIpv4Address : public ZLSocketAddress
// {
// public:
//     ZLIpv4Address(port_t port = 0);
//     ZLIpv4Address(ipaddr_t a,port_t port);
//     ZLIpv4Address(struct in_addr& a,port_t port);
//     ZLIpv4Address(const std::string& host,port_t port);
//     ZLIpv4Address(struct sockaddr_in&);
//     virtual ~ZLIpv4Address();
// 
//     operator struct sockaddr *();
//     operator socklen_t();
//     bool operator==(ZLSocketAddress&);
// 
//     void SetPort(port_t port);
//     port_t GetPort();
// 
//     void SetAddress(struct sockaddr *sa);
//     int GetFamily();
// 
//     bool IsValid();
//     std::auto_ptr<ZLSocketAddress> GetCopy();
// 
//     std::string Convert(bool include_port = false);
//     std::string Reverse();
// 
//     static bool Resolve(const std::string& hostname,struct in_addr& a);
//     static bool Reverse(struct in_addr& a,std::string& name);
//     static std::string Convert(struct in_addr& a);
// 
// private:
//     ZLIpv4Address(const ZLIpv4Address& ) {} // copy constructor
//     ZLIpv4Address& operator=(const ZLIpv4Address& ) { return *this; } // assignment operator
//     struct sockaddr_in m_addr;
//     bool m_valid;
// };
