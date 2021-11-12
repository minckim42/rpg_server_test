#include "Server.hpp"


Server::Server(int port)
:	socket(port)
{}

void		Server::operator()(Server::Controller dummy)
{
	SocketEndpoint*		endpoint;
	OVERLAPPED*			overlapped;
	DWORD				len;

	while (1)
	{
		GetQueuedCompletionStatus(
			socket.completion_port,
			&len,
			reinterpret_cast<PULONG_PTR>(&endpoint),
			reinterpret_cast<LPOVERLAPPED*>(&overlapped),
			INFINITE			
		);
		if (endpoint->is_recv())
		{
			endpoint->len_io += len;
			if (endpoint->len_io < 4)
			{
				endpoint->recv(endpoint->buffer + endpoint->len_io);
				continue;
			}
			if (endpoint->len_io < reinterpret_cast<Message*>(endpoint->buffer)->length)
			{
				endpoint->recv(endpoint->buffer + endpoint->len_io);
				continue;
			}
			endpoint->set_send();
			controller(*(reinterpret_cast<Message*>(endpoint->buffer)));
			endpoint->len_io = 0;
		}
		else
		{
			endpoint->len_io += len;
			int		len_to_send = reinterpret_cast<Message*>(endpoint->buffer)->length;
			int		len_remain = len_to_send - endpoint->len_io;

			if (len_remain > 0)
			{
				endpoint->send(endpoint->buffer + endpoint->len_io, len_remain);
				continue;
			}
			endpoint->set_recv();
			endpoint->len_io = 0;
			endpoint->recv();		
		}
	}
}

void		Server::operator()(Server::Accept dummy)
{
	while (1)
	{
		SocketEndpoint*		endpoint = socket.accept();
		socket.connect_iocp(endpoint);
		endpoint->recv();
	}
}

void		Server::controller(Message& message)
{
	
};