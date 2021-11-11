#include "__SocketBase.hpp"
#include <Ws2tcpip.h>

using namespace std;
/*##############################################################################

	__SocketBase

##############################################################################*/
/*---------------------------------
	Static Members
---------------------------------*/
WSADATA			__SocketBase::wsadata;

//------------------------------------------------------------------------------
void			__SocketBase::init_wsadata()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		throw string("Failed to init WSADATA");
	}
}
//------------------------------------------------------------------------------
void			__SocketBase::cleanup_wsadata()
{
	WSACleanup();
}

/*---------------------------------
	Constructor & Destructor
---------------------------------*/
__SocketBase::__SocketBase()
:	sock(INVALID_SOCKET)
{}
//------------------------------------------------------------------------------
__SocketBase::__SocketBase(__SocketBase&& x)
:	sock(x.sock),
	addr(x.addr)
{
	x.sock = INVALID_SOCKET;
}
//------------------------------------------------------------------------------
__SocketBase::~__SocketBase()
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}
}
//------------------------------------------------------------------------------
__SocketBase&		__SocketBase::operator=(__SocketBase&& x)
{
	if (&x == this)
		return *this;
	sock = x.sock;
	addr = x.addr;
	x.sock = INVALID_SOCKET;
	return *this;
}
//------------------------------------------------------------------------------
void			__SocketBase::set_port(int port)
{
	addr.sin_port = htons(port);
}
//------------------------------------------------------------------------------
void			__SocketBase::set_ip(const string& ip)
{
	if (inet_pton(AF_INET, ip.data(), &addr.sin_addr.S_un.S_addr) == 0)
	{
		throw string("SetIpFailed");
		// throw SetIpFailed();
	}
}
//------------------------------------------------------------------------------
void			__SocketBase::clear_addr()
{
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}















/*---------------------------------
	Exceptions
---------------------------------*/
// const char*		__SocketBase::SocketFailed::what()
// {
// 	return "Fail: socket()";
// }
// const char*		__SocketBase::InitOverlappedFailed::what()
// {
// 	return "Fail: init_overlapped()";
// }
// const char*		__SocketBase::SetIpFailed::what()
// {
// 	return "Fail: set_ip()";
// }
// const char*		__SocketBase::BindFailed::what()
// {
// 	return "Fail: bind()";
// }
// const char*		__SocketBase::ListenFailed::what()
// {
// 	return "Fail: listen()";
// }
// const char*		__SocketBase::AcceptFailed::what()
// {
// 	return "Fail: accept()";
// }