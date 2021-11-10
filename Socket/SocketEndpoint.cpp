#include "SocketEndpoint.hpp"
using namespace std;
/*##############################################################################

	Socket Endpoint

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/
SocketEndpoint::SocketEndpoint()
:	flag(0),
	len_io(0),
	rw_mode(READ)
{
	memset(&overlapped, 0, sizeof(OVERLAPPED));
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv()
{
	wsabuf.buf = buffer;
	wsabuf.len = ENDPOINT_BUF_SIZE;
	return WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv(void* buf, size_t len)
{
	wsabuf.buf = reinterpret_cast<char*>(buf);
	wsabuf.len = len;
	return WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send()
{
	wsabuf.buf = buffer;
	wsabuf.len = ENDPOINT_BUF_SIZE;
	return WSASend(sock, &wsabuf, 1, &len_io, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send(void* buf, size_t len)
{
	wsabuf.buf = reinterpret_cast<char*>(buf);
	wsabuf.len = len;
	return WSASend(sock, &wsabuf, 1, &len_io, flag, &overlapped, NULL);
}