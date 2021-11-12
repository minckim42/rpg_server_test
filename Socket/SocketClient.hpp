#pragma once
#include "__SocketBase.hpp"
#include <string>
/*##############################################################################

	Socket Client

##############################################################################*/
class SocketClient: public __SocketBase
{
	/*---------------------------------
		Member
	---------------------------------*/	
	int			len_read_remain;
	int			len_read;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/	
	public:
	SocketClient();
	SocketClient(const std::string& ip, int port);

	/*---------------------------------
		Methods
	---------------------------------*/	
	void		init();
	void		connect();
	void		connect(const std::string& ip, int port);
	int			recv(void* buffer, int len);
	int			send(void* buffer, int len);
};