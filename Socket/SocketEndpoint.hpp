#include "__SocketBase.hpp"
#include <memory>
#define	ENDPOINT_BUF_SIZE	1024
/*##############################################################################

	Socket Endpoint

##############################################################################*/

class SocketEndpoint: public __SocketBase
{
	/*---------------------------------
		Types
	---------------------------------*/
	public:
	typedef std::shared_ptr<SocketEndpoint>		Ptr;


	/*---------------------------------
		Members
	---------------------------------*/
	enum Mode
	{
		READ = 0,
		WRITE
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
	void		recv();
};