#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <string>
/*##############################################################################

	SocketUDP

##############################################################################*/
#define __UDP_PAYLOAD_SIZE 1000

class SocketUDP
{
	/*---------------------------------
		Static Members
	---------------------------------*/
	static WSADATA	wsadata;

	/*---------------------------------
		Static Methods
	---------------------------------*/
	public:
	static void		init_wsadata();
	static void		cleanup_wsadata();

	/*---------------------------------
		Members
	---------------------------------*/
	public:
	SOCKET			handle;
	SOCKADDR_IN		addr_recv;
	SOCKADDR_IN		addr_send;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	SocketUDP();
	SocketUDP(SocketUDP&& x);
	SocketUDP&		operator=(SocketUDP&& x);
	~SocketUDP();

	/*---------------------------------
		Methods
	---------------------------------*/
	void		init();
	void		set_recv_port(int port);
	void		set_send_port(int port);
	void		set_send_ip(const std::string& ip);
	void		bind(int port);
	int			connect();
	int			connect(const std::string& ip, int port);
	SocketUDP		export_recv_socket(int port = 0);
	int			recvfrom(void* buffer, int size);
	int			sendto(void* buffer, int size);
	int			send(void* buffer, int size);
};
