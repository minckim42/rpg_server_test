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
				service_disconnect(endpoint);
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

	s_message.type = MessageType::RES_CONNECT;
	s_message.set_length<ResConnect>();

	LocalData	player_data = load_player_data(name, password);

	if (player_data.size() == 0)
	{
		s_message.get_body<ResConnect>().result = ResConnect::LOGIN_FAIL;
		print_log("Login Failed: " + name);
		return;
	}

	PlayerBase		tmp = init_player(player_data);

	if (players.find(tmp.id) != players.end())
	{
		s_message.get_body<ResConnect>().result = ResConnect::LOGIN_FAIL;
		print_log("Login Failed: Already logged in: " + name);
		return;
	}

	sock_id_map[endpoint->sock] = tmp.id;
	players[tmp.id] = tmp;

	s_message.get_body<ResConnect>().result = ResConnect::LOGIN_SUCCESS;
	s_message.get_body<ResConnect>().player = tmp.cast_response();

	print_log("Login success: " + name);
}
//------------------------------------------------------------------------------
void			Server::service_disconnect(SocketEndpoint* endpoint)
{
	print_log(string("Disconnected: ") + to_string(endpoint->sock));
	
	if (sock_id_map.find(endpoint->sock) != sock_id_map.end())
	{
		uint32_t	id = sock_id_map[endpoint->sock];
		sock_id_map.erase(endpoint->sock);

		print_log(string("erase id from map: ") + to_string(endpoint->sock));
		if (players.find(id) != players.end())
		{
			players.erase(id);
			print_log(string("erase player from map: ") + to_string(id));
		}
	}
	delete endpoint;
}
//------------------------------------------------------------------------------
Server::LocalData	Server::load_player_data(const string& name, const string& password)
{
	string	query = \
	R"asdf(
		SELECT
			idplayer,
			name,
			position_x,
			position_y,
			direction_x,
			direction_y,
			speed,
			shape
		FROM player
		WHERE name=')asdf";
	query += name.data() + string("' AND password='") + password.data() + "';";
	return database.select(query);
}
//------------------------------------------------------------------------------
PlayerBase			Server::init_player(Database::SelectData& data)
{
	enum {
		ID_PLAYER=0,
		NAME,
		POSITION_X,
		POSITION_Y,
		DIRECTION_X,
		DIRECTION_Y,
		SPEED,
		SHAPE
	};
	#ifdef __DATABASE
	vector<string>	row = data[0];
	#endif
	return PlayerBase(
		Movable(
			Vec2(stof(row[POSITION_X]), stof(row[POSITION_Y])),
			Vec2(stof(row[DIRECTION_X]), stof(row[DIRECTION_Y])),
			stof(row[SPEED])
		),
		stol(row[ID_PLAYER]),
		row[NAME],
		row[SHAPE][0]
	);
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