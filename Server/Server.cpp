#include "Server.hpp"

using namespace std;
/*##############################################################################

	Server

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/	
Server::Server(int port)
:	socket(port)
{
	database.init("127.0.0.1", "root", "1234", "rpg_test");
	cout << "database init\n";
}

/*---------------------------------
	Methods
---------------------------------*/
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
			print_log("RECV mode");
			// Disconnected
			if (len == 0)
			{
				print_log(string("Disconnected: ") + to_string(endpoint->sock));
				delete endpoint;
				continue;
			}

			endpoint->len_recv += len;
			if (endpoint->len_recv < 4)
			{
				endpoint->recv(endpoint->buffer_recv + endpoint->len_recv);
				continue;
			}
			if (endpoint->len_recv < get_recv_message(endpoint).length)
			{
				endpoint->recv(endpoint->buffer_recv + endpoint->len_recv);
				continue;
			}
			
			controller(endpoint);

			endpoint->set_send();
			endpoint->send(get_send_message(endpoint).length);
			endpoint->len_send = 0;
		}
		else
		{
			print_log("SEND mode");
			endpoint->len_send += len;
			int		len_to_send = get_send_message(endpoint).length;
			int		len_remain = len_to_send - endpoint->len_send;

			if (len_remain > 0)
			{
				endpoint->send(endpoint->buffer_send + endpoint->len_send, len_remain);
				continue;
			}
			endpoint->set_recv();
			memmove(
				endpoint->buffer_recv,
				endpoint->buffer_recv + get_recv_message(endpoint).length,
				endpoint->len_recv - get_recv_message(endpoint).length
			);
			endpoint->len_recv -= get_recv_message(endpoint).length;
			endpoint->recv();
		}
	}
}
//------------------------------------------------------------------------------
void		Server::operator()(Server::Accept dummy)
{
	while (1)
	{
		SocketEndpoint*		endpoint = socket.accept();
		print_log(string("Accept ") + to_string(endpoint->sock));
		socket.connect_iocp(endpoint);
		endpoint->recv();
	}
}
//------------------------------------------------------------------------------
void		Server::controller(SocketEndpoint* endpoint)
{
	print_log(__func__);
	Message&	message = get_recv_message(endpoint);
	switch (message.type)
	{
		case MessageType::REQ_LOGIN:
			service_login(endpoint);
			break;
	}	
};
//------------------------------------------------------------------------------
void		Server::service_login(SocketEndpoint* endpoint)
{
	print_log(__func__);

	Message&	r_message = get_recv_message(endpoint);
	Message&	s_message = get_send_message(endpoint);

	string	name = r_message.get_body<ReqLogin>().name;
	string	password = r_message.get_body<ReqLogin>().password;

	string	query = \
		R"asdf(SELECT idplayer, position_x, position_y, direction_x, direction_y, speed
		FROM player
		WHERE name=')asdf";
		query += name.data() + string("' AND password='") + password.data() + "';";

	auto	result = database.select(query);

	s_message.type = MessageType::RES_CONNECT;
	s_message.set_length<ResConnect>();
	if (result.size() == 0)
	{
		s_message.get_body<ResConnect>().result = ResConnect::LOGIN_FAIL;
		s_message.get_body<ResConnect>().user_id = 0;
		print_log("Login Failed: " + name);
	}
	else
	{
		PlayerBase	tmp(
			Movable(
				Vec2(stof(result[0][1]), stof(result[0][2])),				
				Vec2(stof(result[0][3]), stof(result[0][4])),
				stof(result[0][5])
			),
			stol(result[0][0]),
			name
		);

		s_message.get_body<ResConnect>().result = ResConnect::LOGIN_SUCCESS;
		s_message.get_body<ResConnect>().user_id = tmp.id;

		print_log("Login success: " + name);
	}
}
//------------------------------------------------------------------------------
Message&		Server::get_recv_message(SocketEndpoint* endpoint)
{
	return reinterpret_cast<Message&>(*endpoint->buffer_recv);
}
//------------------------------------------------------------------------------
Message&		Server::get_send_message(SocketEndpoint* endpoint)
{
	return reinterpret_cast<Message&>(*endpoint->buffer_send);
}
//------------------------------------------------------------------------------
void		print_log(const string& str)
{
	cout << "LOG | " << str << endl;
}
//------------------------------------------------------------------------------