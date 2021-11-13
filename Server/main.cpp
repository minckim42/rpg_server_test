#include "../Socket/SocketIOCP.hpp"
#include "Server.hpp"
#include <iostream>

using namespace std;

int			main(int argc, char** argv)
{
	try
	{
		init_wsadata();

		Server		server(9090);

		server.socket.bind();
		server.print_log("bind OK");
		server.socket.listen(5);
		server.print_log("listen OK");
		server.socket.create_iocp();
		server.print_log("create iocp OK");
		server.socket.init_thread(ref(server), Server::Controller());
		server.print_log("create threads OK");
		server(Server::Accept());
		cleanup_wsadata();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	catch(const std::string& e)
	{
		std::cerr << e << '\n';
	}
	return 0;
}









// template	<typename T, typename Q>
// class	ThreadMain
// {
// 	public:
// 	void	operator()(HANDLE completion_port_io, int val, T& a)
// 	{
// 		SocketEndpoint*		endpoint;
// 		OVERLAPPED*			overlapped;
// 		DWORD				len;
// 		SOCKET*				sock;

// 		a();

// 		while (1)
// 		{
// 			GetQueuedCompletionStatus(
// 				completion_port_io,
// 				&len,
// 				reinterpret_cast<PULONG_PTR>(&endpoint),
// 				reinterpret_cast<LPOVERLAPPED*>(&overlapped),
// 				INFINITE
// 			);
// 			cout << "passed queue:" << val << endl;
// 			if (endpoint->rw_mode == SocketEndpoint::READ)
// 			{
// 				if (len == 0)
// 				{
// 					cout << "disconnected: " << endpoint->sock << endl;
// 					closesocket(endpoint->sock);
// 					delete endpoint;
// 					continue;
// 				}
// 				cout << "message received\n";
// 				cout << "	len: " << len << endl;
// 				cout << "	endpoint.len_io: " << endpoint->len_io << endl;
// 				cout << "	endpoint.wsa.len: " << endpoint->wsabuf.len << endl;
// 				cout << "	content: ";
// 				cout.write(endpoint->wsabuf.buf, len);
// 				cout << endl;

// 				endpoint->wsabuf.len = len;
// 				endpoint->len_io = len;
// 				endpoint->rw_mode = SocketEndpoint::WRITE;
// 				// cout << "	send:" << endpoint->send((char*)"hello", 6) << endl;
// 				cout << "	send:" << endpoint->send(len) << endl;;
// 			}
// 			else
// 			{
// 				cout << "sending! ";
// 				endpoint->rw_mode = SocketEndpoint::READ;
				
// 				cout << endpoint->recv() << endl;
// 			}
// 			cout << "end_cycle\n";
// 		}
// 	}
// };

// class		Foo
// {
// 	public:
// 	void	operator()()
// 	{
// 		cout << "foo!!!" << this << endl;
// 	}
// };

// int			main()
// {
// 	__SocketBase::init_wsadata();
// 	try {

// 		SocketIOCP		so(9090);

// 		int				val;


// 		ThreadMain<Foo, int>		thread_main;

// 		Foo		foo;
// 		so.create_iocp();
// 		so.init_thread(thread_main, so.completion_port, val, ref(foo));

// 		so.bind();
// 		cout << "bind\n";
// 		so.listen(5);
// 		cout << "listen\n";
// 		while (1)
// 		{
// 			SocketEndpoint*	endpoint = so.accept();
// 			cout << "accept: " << endpoint->sock << endl;
// 			so.connect_iocp(endpoint);
// 			endpoint->recv();
// 		}
// 	}
// 	catch (string& err)
// 	{
// 		cout << err;
// 	}
// }