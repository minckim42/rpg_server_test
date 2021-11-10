#include "../Socket/SocketIOCP.hpp"
#include <iostream>

using namespace std;

void	echo_thread_main(HANDLE completion_port_io)
{
	SocketEndpoint*		endpoint;
	DWORD				len;
	SOCKET* sock;

	while (1)
	{
		GetQueuedCompletionStatus(
			completion_port_io,
			&len,
			reinterpret_cast<PULONG_PTR>(&sock),
			reinterpret_cast<LPOVERLAPPED*>(&endpoint),
			INFINITE
		);

		if (endpoint->rw_mode == SocketEndpoint::READ)
		{
			if (endpoint->len_io == 0)
			{
				closesocket(endpoint->sock);
				delete endpoint;
				cout << "disconnected: " << endpoint->sock << endl;
				continue;
			}

			cout << "message received: \n";
		}
	}
}

int			main()
{
	__SocketBase::init_wsadata();
	try {

		SocketIOCP		so(9090);

		so.create_iocp();
		//so.init_thread(echo_thread_main, so.completion_port);

		SYSTEM_INFO			sys_info;
		GetSystemInfo(&sys_info);

		for (unsigned int i = 0; i < sys_info.dwNumberOfProcessors; i++)
		{
			so.threads.emplace_back(echo_thread_main, so.completion_port);
			so.threads.back().detach();
		}


		so.bind();
		so.listen(5);
		while (1)
		{
			SocketEndpoint*	endpoint = so.accept();
			so.connect_iocp(endpoint);
		}
	}
	catch (string& err)
	{
		cout << err;
	}
}