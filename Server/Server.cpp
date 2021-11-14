#include "Server.hpp"
#include <iomanip>

using namespace std;
/*##############################################################################

	Server

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/	
Server::Server(int port)
:	socket(port),
	time_start(clock::now())
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
			print_log(string("remain: ") + to_string(endpoint->len_recv));
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

	if (!message.check_hash())
	{
		print_log("Hash Error");
		return;
	}

	switch (message.type)
	{
		case MessageType::REQ_LOGIN:
			service_login(endpoint);
			break;

		case MessageType::REQ_GAME:
			service_game(endpoint);
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

	
	s_message.get_body<ResConnect>().player = init_player(player_data);
	PlayerBase&		tmp = s_message.get_body<ResConnect>().player;

	if (players.find(tmp.id) != players.end())
	{
		s_message.get_body<ResConnect>().result = ResConnect::LOGIN_FAIL;
		print_log("Login Failed: Already logged in: " + name);
		return;
	}

	sock_id_map[endpoint->sock] = tmp.id;
	players[tmp.id] = tmp;

	s_message.get_body<ResConnect>().result = ResConnect::LOGIN_SUCCESS;

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
void			Server::service_game(SocketEndpoint* endpoint)
{
	Message&	r_message = get_recv_message(endpoint);
	Message&	s_message = get_send_message(endpoint);

	ReqGame&	req_game = r_message.get_body<ReqGame>();
	BulletBase*	r_bullets = &(r_message.get_body<ReqGame, BulletBase>());

	memset(&s_message, 0, sizeof(Message));

	// find player and update
	
	// lock_guard<mutex>	g(mutex_players);

	print_log(string("ReqGame: ") + to_string(req_game.player.id));
	if (set_player(&req_game.player) == false)
		return;

	// bullet update
	for (int i = 0 ; i < req_game.len_bullets ; i++)
	{
		set_bullet(r_bullets + i);
		if (r_bullets[i].id_hit != 0)
			set_player_dead(r_bullets[i].id_hit);
	}

	s_message.type = MessageType::RES_GAME;

	PlayerBase*	s_players = &(s_message.get_body<ResGame, PlayerBase>());
	BulletBase*	s_bullets = &(s_message.get_body<ResGame, PlayerBase, BulletBase>(players.size()));

	vector<uint32_t>		bullets_to_be_erased;

	int i = 0;
	auto	it_player = players.begin();
	while (it_player != players.end())
	{
		if (it_player->second.is_alive == false)
		{
			it_player->second.shape = '#';
		}
		s_players[i] = it_player->second;
		s_players[i++].time_send = time_now();
		++it_player;
	}
	i = 0;
	auto	it_bullet = bullets.begin();
	while (it_bullet != bullets.end())
	{
		if (time_now() - it_bullet->second.time_birth > 4000)
		{
			it_bullet = bullets.erase(it_bullet);
			continue;
		}
		s_bullets[i] = it_bullet->second;
		s_bullets[i++].time_send = time_now();
		++it_bullet;
	}
	s_message.set_length<ResGame, PlayerBase, BulletBase>(players.size(), bullets.size());
	s_message.get_body<ResGame>().len_bullets = bullets.size();
	s_message.get_body<ResGame>().len_players = players.size();
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
		ID=0,
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
	PlayerBase	result;
	result.movable.position = Vec2(stof(row[POSITION_X]), stof(row[POSITION_Y]));
	result.movable.direction = Vec2(stof(row[DIRECTION_X]), stof(row[DIRECTION_Y]));
	result.movable.speed = 0;
	result.id = stol(row[ID]);
	result.time_recv = time_now();
	result.time_send = time_now();
	result.shape = row[SHAPE][0];
	result.is_moving = false;
	result.is_alive = true;
	result.speed = stof(row[SPEED]);
	memset(&result.name, 0, LEN_NAME);
	memcpy(&result.name, row[NAME].data(), row[NAME].length());
	return result;
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
double			Server::time_now()
{
	return static_cast<double>(chrono::duration_cast<chrono::microseconds>
		(clock::now() - time_start).count()) / 1000;
}
//------------------------------------------------------------------------------
void			Server::print_log(const string& str)
{
	cout << "LOG | " << setw(8) << setprecision(3) << time_now() / 1000 << "s | " << str << endl;
}
//------------------------------------------------------------------------------
bool			Server::set_player(PlayerBase* r_player)
{
	auto	it = players.find(r_player->id);
	if (it == players.end())
		return false;
	bool	tmp = it->second.is_alive;
	it->second = *r_player;
	it->second.is_alive = tmp;
	it->second.time_recv = time_now();
	return true;
}
//------------------------------------------------------------------------------
void			Server::set_bullet(BulletBase* r_bullet)
{
	if (r_bullet->time_birth == 0)
	{
		r_bullet->time_birth = time_now();
	}
	bullets[r_bullet->id] = *r_bullet;
	bullets[r_bullet->id].time_recv = time_now();
}
//------------------------------------------------------------------------------
bool			Server::set_player_dead(uint32_t key)
{
	auto	it = players.find(key);
	if (it == players.end())
		return false;
	it->second.is_alive = false;
	return true;
}
//------------------------------------------------------------------------------