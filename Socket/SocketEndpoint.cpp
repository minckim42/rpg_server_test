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
	rw_mode(RECV)
{
	memset(&overlapped, 0, sizeof(OVERLAPPED));
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv()
{
	wsabuf.buf = buffer;
	wsabuf.len = ENDPOINT_BUF_SIZE;
	len_io = 0;
	return WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv(void* buf_extra)
{
	wsabuf.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf.len = ENDPOINT_BUF_SIZE - len_io;
	return WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv(void* buf_extra, size_t len)
{
	wsabuf.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf.len = len;
	return WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send()
{
	wsabuf.buf = buffer;
	// wsabuf.len = ENDPOINT_BUF_SIZE;
	return WSASend(sock, &wsabuf, 1, &len_io, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send(void* buf_extra, size_t len)
{
	wsabuf.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf.len = len;
	return WSASend(sock, &wsabuf, 1, &len_io, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send(size_t len)
{
	wsabuf.buf = buffer;
	wsabuf.len = len;
	return WSASend(sock, &wsabuf, 1, &len_io, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
void		SocketEndpoint::set_recv()
{
	rw_mode = RECV;
}
//------------------------------------------------------------------------------
void		SocketEndpoint::set_send()
{
	rw_mode = SEND;
}
//------------------------------------------------------------------------------
bool		SocketEndpoint::is_recv()
{
	return rw_mode == RECV;
}
//------------------------------------------------------------------------------
bool		SocketEndpoint::is_send()
{
	return rw_mode == SEND;
}
//------------------------------------------------------------------------------