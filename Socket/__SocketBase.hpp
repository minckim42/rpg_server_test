#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>
#include <string>

class __SocketBase
{
	/*---------------------------------
		Static Members
	---------------------------------*/
	static WSADATA			wsadata;

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
	SOCKET			sock;
	SOCKADDR_IN		addr;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	private:
	__SocketBase(const __SocketBase&);
	__SocketBase&		operator=(const __SocketBase&);

	public:
	__SocketBase();
	__SocketBase(SOCKET sock, SOCKADDR_IN addr);
	__SocketBase(__SocketBase&& x);
	virtual			~__SocketBase();
	__SocketBase&		operator=(__SocketBase&& x);

	/*---------------------------------
		Methods
	---------------------------------*/
	void		set_ip(const std::string& ip);
	void		set_port(int port);
	void		clear_addr();
};

	// /*---------------------------------
	// 	Exceptions
	// ---------------------------------*/
	// class SocketFailed: public std::exception {
	// 	const char*		what();
	// };
	// class InitOverlappedFailed: public std::exception {
	// 	const char*		what();
	// };
	// class SetIpFailed: public std::exception {
	// 	const char*		what();
	// };
	// class BindFailed: public std::exception {
	// 	const char*		what();
	// };
	// class ListenFailed: public std::exception {
	// 	const char*		what();
	// };
	// class AcceptFailed: public std::exception {
	// 	const char*		what();
	// };

/*---------------------------------
	Non Member
---------------------------------*/
void		init_wsadata();
void		cleanup_wsadata();
