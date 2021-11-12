#include "SocketClient.hpp"

using namespace std;
/*##############################################################################

	Socket Client

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/	
SocketClient::SocketClient()
:	__SocketBase()
{}
//------------------------------------------------------------------------------
SocketClient::SocketClient(const std::string& ip, int port)
:	__SocketBase()
{
	init();
	connect(ip, port);
}
//------------------------------------------------------------------------------
void		SocketClient::init()
{
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		throw string("SocketFailed");
		// throw SocketFailed();
	}
	clear_addr();
	addr.sin_family = AF_INET;
}
//------------------------------------------------------------------------------
void		SocketClient::connect()
{
	if (::connect(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR_IN))
	== SOCKET_ERROR)
		throw string("Error: connect()");
}
//------------------------------------------------------------------------------
void		SocketClient::connect(const string& ip, int port)
{
	set_ip(ip);
	set_port(port);
	connect();
}
//------------------------------------------------------------------------------
int			SocketClient::recv(void* buffer, int len)
{
	return ::recv(sock, reinterpret_cast<char*>(buffer), len, 0);
}
//------------------------------------------------------------------------------
int			SocketClient::send(void* buffer, int len)
{
	return ::send(sock, reinterpret_cast<char*>(buffer), len, 0);
}