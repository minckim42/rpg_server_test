#include "Socket.hpp"
#include <Ws2tcpip.h>

using namespace std;
/*##############################################################################

	Socket

##############################################################################*/
/*---------------------------------
	Static Members
---------------------------------*/
WSADATA		Socket::wsadata;

//------------------------------------------------------------------------------
void		Socket::init_wsadata()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		throw string("Failed to init WSADATA");
	}
}
//------------------------------------------------------------------------------
void		Socket::cleanup_wsadata()
{
	WSACleanup();
}

/*---------------------------------
	Constructor & Destructor
---------------------------------*/
Socket::Socket()
:	handle(INVALID_SOCKET)
{}
//------------------------------------------------------------------------------
Socket::Socket(Socket&& x)
:	handle(x.handle),
	addr_recv(x.addr_recv),
	addr_send(x.addr_send)
{
	x.handle = INVALID_SOCKET;
}
//------------------------------------------------------------------------------
Socket&		Socket::operator=(Socket&& x)
{
	if (&x == this)
		return *this;
	handle = x.handle;
	addr_recv = x.addr_recv;
	addr_send = x.addr_send;
	x.handle = INVALID_SOCKET;
	return *this;
}
//------------------------------------------------------------------------------
Socket::~Socket()
{
	if (handle != INVALID_SOCKET)
		closesocket(handle);
}

/*---------------------------------
	Methods
---------------------------------*/
void		Socket::init()
{
	handle = socket(PF_INET, SOCK_DGRAM, 0);
	if (handle == INVALID_SOCKET)
	{
		throw string("Failed to init socket");
	}
	memset(&addr_send, 0, sizeof(addr_send));
	memset(&addr_recv, 0, sizeof(addr_recv));
	addr_send.sin_family = AF_INET;
	addr_recv.sin_family = AF_INET;
}
//------------------------------------------------------------------------------
void		Socket::set_recv_port(int port)
{
	addr_recv.sin_port = htons(port);
}
//------------------------------------------------------------------------------
void		Socket::set_send_port(int port)
{
	addr_send.sin_port = htons(port);
}
//------------------------------------------------------------------------------
void		Socket::set_send_ip(const string& ip)
{
	if (inet_pton(AF_INET, ip.data(), &addr_send.sin_addr.S_un.S_addr) == 0)
	{
		throw std::string("Error: inet_pton()");
	}
}
//------------------------------------------------------------------------------
void		Socket::bind(int port)
{
	init();

	addr_recv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	set_recv_port(port);

	if (::bind(handle, 
		reinterpret_cast<SOCKADDR*>(&addr_recv),
		sizeof(addr_recv)
		) == SOCKET_ERROR)
	{
		closesocket(handle);
		throw string("Failed to bind socket");
	}
}
//------------------------------------------------------------------------------
int			Socket::connect()
{
	return ::connect(handle, reinterpret_cast<SOCKADDR*>(&addr_send), sizeof(SOCKADDR_IN));
}
//------------------------------------------------------------------------------
int			Socket::connect(const string& ip, int port)
{
	init();
	set_send_ip(ip);
	set_send_port(port);
	return connect();
}
//------------------------------------------------------------------------------

Socket		Socket::export_recv_socket(int port)
{
	Socket	tmp;
	tmp.init();
	tmp.addr_send = addr_send;
	if (port)
		tmp.set_send_port(port);
	tmp.connect();
	return tmp;
}

//------------------------------------------------------------------------------
int			Socket::recvfrom(void* buffer, int size)
{
	int			size_address = sizeof(SOCKADDR_IN);

	return ::recvfrom(
		handle,
		reinterpret_cast<char*>(buffer),
		size,
		0,
		reinterpret_cast<SOCKADDR*>(&addr_send),
		&size_address
	);
}
//------------------------------------------------------------------------------
int			Socket::sendto(void* buffer, int size)
{
	return ::sendto(
		handle,
		reinterpret_cast<char*>(buffer),
		size,
		0,
		reinterpret_cast<SOCKADDR*>(&addr_send),
		sizeof(SOCKADDR_IN)
	);
}
//------------------------------------------------------------------------------
int			Socket::send(void* buffer, int size)
{
	return ::send(handle, reinterpret_cast<char*>(buffer), size, 0);
}
//------------------------------------------------------------------------------