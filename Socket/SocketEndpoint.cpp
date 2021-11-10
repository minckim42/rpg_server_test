#include "SocketEndpoint.hpp"
using namespace std;
/*##############################################################################

	Socket Endpoint

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/
SocketEndpoint::SocketEndpoint()
:	flag(0)
{
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	wsabuf.buf = buffer;
	wsabuf.len = ENDPOINT_BUF_SIZE;
	rw_mode = WRITE;
}
//------------------------------------------------------------------------------
void		SocketEndpoint::recv()
{
	WSARecv(sock, &wsabuf, 1, &len_io, &flag, &overlapped, NULL);
}