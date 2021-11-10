#include "__SocketBase.hpp"

class SocketClient: public __SocketBase
{
	void		init();
	int			connect();
	int			connect(const std::string& ip, int port);
};