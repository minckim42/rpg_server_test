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

SocketClient::SocketClient(const std::string& ip, int port)
:	__SocketBase()
{
	clear_addr();
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
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
}
//------------------------------------------------------------------------------
int			SocketClient::connect()
{
	return ::connect(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR_IN));
}
//------------------------------------------------------------------------------
int			SocketClient::connect(const string& ip, int port)
{
	set_ip(ip);
	set_port(port);
	return connect();
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