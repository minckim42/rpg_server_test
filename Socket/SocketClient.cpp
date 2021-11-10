#include "SocketClient.hpp"

// //------------------------------------------------------------------------------
// void		SocketClient::init()
// {
// 	sock = socket(PF_INET, SOCK_STREAM, 0);
// 	if (sock == INVALID_SOCKET)
// 	{
// 		throw string("SocketFailed");
// 		// throw SocketFailed();
// 	}
// 	memset(&addr, 0, sizeof(SOCKADDR_IN));
// 	addr.sin_family = AF_INET;
// }
// //------------------------------------------------------------------------------
// int			SocketClient::connect()
// {
// 	return ::connect(sock, reinterpret_cast<SOCKADDR*>(&addr_send), sizeof(SOCKADDR_IN));
// }
// //------------------------------------------------------------------------------
// int			SocketClient::connect(const string& ip, int port)
// {
// }
