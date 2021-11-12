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
	len_send(0),
	len_recv(0),
	rw_mode(RECV)
{
	memset(&overlapped, 0, sizeof(OVERLAPPED));
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv()
{
	wsabuf_recv.buf = buffer_recv;
	wsabuf_recv.len = ENDPOINT_BUF_SIZE;
	len_recv = 0;
	return WSARecv(sock, &wsabuf_recv, 1, &len_recv, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv(void* buf_extra)
{
	wsabuf_recv.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf_recv.len = ENDPOINT_BUF_SIZE - len_recv;
	return WSARecv(sock, &wsabuf_recv, 1, &len_recv, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::recv(void* buf_extra, size_t len)
{
	wsabuf_recv.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf_recv.len = len;
	return WSARecv(sock, &wsabuf_recv, 1, &len_recv, &flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send()
{
	wsabuf_send.buf = buffer_send;
	// wsabuf_send.len = ENDPOINT_BUF_SIZE;
	return WSASend(sock, &wsabuf_send, 1, &len_send, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send(void* buf_extra, size_t len)
{
	wsabuf_send.buf = reinterpret_cast<char*>(buf_extra);
	wsabuf_send.len = len;
	return WSASend(sock, &wsabuf_send, 1, &len_send, flag, &overlapped, NULL);
}
//------------------------------------------------------------------------------
int			SocketEndpoint::send(size_t len)
{
	wsabuf_send.buf = buffer_send;
	wsabuf_send.len = len;
	return WSASend(sock, &wsabuf_send, 1, &len_send, flag, &overlapped, NULL);
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