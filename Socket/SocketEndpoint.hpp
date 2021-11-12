#pragma once
#include "__SocketBase.hpp"
#include <memory>
#define	ENDPOINT_BUF_SIZE	1024
/*##############################################################################

	Socket Endpoint

##############################################################################*/

class SocketEndpoint: public __SocketBase
{
	/*---------------------------------
		Members
	---------------------------------*/
	public:
	enum Mode
	{
		RECV = 0,
		SEND
	};

	OVERLAPPED		overlapped;
	char			buffer[ENDPOINT_BUF_SIZE];
	WSABUF			wsabuf;
	Mode			rw_mode;
	DWORD			len_io;
	DWORD			flag;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	SocketEndpoint();

	/*---------------------------------
		Methods
	---------------------------------*/
	int			recv();
	int			recv(void* buf_extra);
	int			recv(void* buf_extra, size_t len);
	int			send();
	int			send(size_t len);
	int			send(void* buf_extra, size_t len);

	void		set_recv();
	void		set_send();
	bool		is_recv();
	bool		is_send();
};