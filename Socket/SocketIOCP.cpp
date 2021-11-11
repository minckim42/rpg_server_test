#include "SocketIOCP.hpp"
using namespace std;
/*##############################################################################

	SocketIOCP

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/
SocketIOCP::SocketIOCP(int port)
:	__SocketBase()
{
	sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET)
	{
		throw string("InitOverlappedFailed");
		// throw InitOverlappedFailed();
	}
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	set_port(port);
}
//------------------------------------------------------------------------------
void			SocketIOCP::create_iocp()
{
	completion_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}
//------------------------------------------------------------------------------
void		SocketIOCP::bind()
{
	if (::bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) \
	== SOCKET_ERROR)
	{
		throw string("BindFailed");
		// throw BindFailed();
	}
}
//------------------------------------------------------------------------------
void		SocketIOCP::listen(int backlog)
{
	if (::listen(sock, backlog) == SOCKET_ERROR)
	{
		throw string("ListenFailed");
		// throw ListenFailed();
	}
}
//------------------------------------------------------------------------------
SocketEndpoint*		SocketIOCP::accept()
{
	SocketEndpoint*		endpoint = new SocketEndpoint;
	int					len_addr = sizeof(SOCKADDR_IN);
	endpoint->sock = ::accept(sock,	reinterpret_cast<SOCKADDR*>(&endpoint->addr), &len_addr);
	if (endpoint->sock == INVALID_SOCKET)
	{
		throw string("AcceptFailed");
		// throw AcceptFailed();
	}
	return endpoint;

}
//------------------------------------------------------------------------------
void				SocketIOCP::connect_iocp(SocketEndpoint* endpoint)
{
	CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(endpoint->sock),
		completion_port,
		reinterpret_cast<ULONG_PTR>(endpoint),
		0
	);
}
//------------------------------------------------------------------------------
