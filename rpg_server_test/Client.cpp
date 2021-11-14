#include "Client.hpp"
#include <iostream>
using namespace std;

/*##############################################################################

	Client

##############################################################################*/
Client::Client(const string& ip, int port)
:	socket(ip, port),
	status(Status::START),
	time_start(),
	screen(SCREEN_SIZE)
{}
//------------------------------------------------------------------------------
void		Client::run()
{
	while (1)
	{
		switch (status)
		{
		case Status::START:
			start();
			break;

		case Status::LOGIN_WAIT:
			connect_wait();
			break;

		case Status::SIGN_UP:
			sign_up();
			break;

		case Status::SIGN_UP_WAIT:
			connect_wait();
			break;

		case Status::GAME:
			game();
			break;

		case Status::DEAD:
			dead();
			break;

		case Status::EXIT:
			return;
			break;
		
		default:
			return;
			break;
		}
	}
}
//------------------------------------------------------------------------------
void		Client::operator()(Client::Controller dummy)
{
	int		len_read = 0;
	Message	message;

	while (1)
	{
		len_read += socket.recv(
			reinterpret_cast<char*>(&message) + len_read, sizeof(Message));
		if (len_read < sizeof(uint32_t))
			continue;
		if (len_read < message.length)
			continue;
		if (len_read == SOCKET_ERROR || status == Status::EXIT)
			return;
		controller(message);
		memmove(&message, &message + message.length, len_read - message.length);
		len_read -= message.length;
	}
}
//------------------------------------------------------------------------------
void		Client::controller(Message& message)
{
	switch (message.type)
	{
		case MessageType::RES_CONNECT:
			service_connect(message);
			break;

		case MessageType::RES_LOG_IN_FAIL:
			service_log_in_fail(message);
			break;

		case MessageType::RES_SIGN_UP_FAIL:
			service_sign_up_fail(message);
			break;

		case MessageType::RES_GAME:
			service_game(message);
			break;
	}
}
//------------------------------------------------------------------------------
void		Client::game()
{
	KeyManager	k_move(VK_UP);
	KeyManager	k_turn_left(VK_LEFT);
	KeyManager	k_turn_right(VK_RIGHT);
	KeyManager	k_shot(VK_SPACE);
	KeyManager	k_exit0('Q');
	KeyManager	k_exit1(VK_ESCAPE);

	FrameManager	screen_manager(SCREEN_INTERVAL);
	FrameManager	message_manager(MESSAGE_INTERVAL);

	double		time0 = time_now();
	double		last_shot = 0;
	uint32_t	bullet_count = 0;
	
	while (1)
	{
		if (status == Status::DEAD)
			return;
		double	time_cur = time_now();

		me.movable.speed = 0;
		// instruction
		double	time1 = time_now();
		if (k_turn_left.is_pressed())
			me.movable.rotate((time_cur - time0) / 1000);
		if (k_turn_right.is_pressed())
			me.movable.rotate(-(time_cur - time0) / 1000);
		if (k_move.is_pressed())
			me.movable.speed = me.speed;
		if (k_shot.is_pressed())
		{
			if (time_now() - last_shot > BULLET_COOLDOWN)
			{
				shot(bullet_count);
				last_shot = time_now();
			}
		}
		if (k_exit0.is_pressed() || k_exit1.is_pressed())
		{
			status = Status::EXIT;
			return;
		}

		update(time_cur - time0);
		time0 = time_cur;

		message_manager.set_interval(*this, SendMessage());
		screen_manager.set_interval(*this, PrintScreen());
	}
}
//------------------------------------------------------------------------------
void		Client::shot(uint32_t& bullet_count)
{
	uint32_t	bullet_id = me.id * 0x1000 + bullet_count++;
	
	BulletBase&	bullet = (my_bullets[bullet_id] = BulletBase());
	bullet.movable = me.movable;
	bullet.movable.speed = BULLET_SPEED;
	bullet.time_birth = 0;
	bullet.time_recv =0;
	bullet.time_send = 0;
	bullet.id = bullet_id;
	bullet.id_hit = 0;
}
//------------------------------------------------------------------------------
void		Client::update(double time)
{
	lock_guard<mutex>	g(mutex_update);
	players.erase(me.id);

	auto	it_my_bullet = my_bullets.begin();
	while (it_my_bullet != my_bullets.end())
	{
		if (it_my_bullet->second.time_birth != 0
		&& time_now() - it_my_bullet->second.time_birth > BULLET_LIFE)
		{
			it_my_bullet = my_bullets.erase(it_my_bullet);
			continue;
		}
		for (auto& player : players)
		{
			if (player.second.movable.is_collide(it_my_bullet->second.movable, time, 1))
			{
				it_my_bullet->second.id_hit = player.second.id;
			}
		}
		it_my_bullet->second.movable.update(time);
		if (bullets.find(it_my_bullet->second.id) != bullets.end())
		{
			bullets.erase(it_my_bullet->second.id);
		}
		++it_my_bullet;
	}
	for (auto& i : players)
	{
		i.second.movable.update(time);
	}
	for (auto& i : bullets)
	{
		i.second.movable.update(time);
	}
	me.movable.update(time);
}

//------------------------------------------------------------------------------
void		Client::service_connect(Message& message)
{
	ResConnect	res = message.get_body<ResConnect>();
	me = res.player;
	cout << "Login Success" << endl;
	status = Status::GAME;
}
//------------------------------------------------------------------------------
void		Client::service_log_in_fail(Message& message)
{
	cout << "Log in Failed" << endl;
	status = Status::START;
}
//------------------------------------------------------------------------------
void		Client::service_sign_up_fail(Message& message)
{
	cout << "Sign up Failed" << endl;
	status = Status::START;
}
//------------------------------------------------------------------------------
void		Client::service_game(Message& message)
{
	ResGame&	res = message.get_body<ResGame>();
	PlayerBase*	r_players = &(message.get_body<ResGame, PlayerBase>());
	BulletBase*	r_bullets = &(message.get_body<ResGame, PlayerBase, BulletBase>(res.len_players));

	lock_guard<mutex>	g(mutex_update);
	players.clear();
	bullets.clear();
	for (int i = 0 ; i < res.len_players ; i++)
	{
		if (me.id == r_players[i].id && !r_players[i].is_alive)
		{
			me.is_alive = false;
			status = Status::DEAD;
			return;
		}
		double	time = r_players[i].time_send - r_players[i].time_recv;
		r_players[i].movable.update(time);
		players[r_players[i].id] = r_players[i];
	}
	for (int i = 0 ; i < res.len_bullets ; i++)
	{
		double	time = r_bullets[i].time_send - r_bullets[i].time_recv;
		r_bullets[i].movable.update(time);
		bullets[r_bullets[i].id] = r_bullets[i];
	}
	// mutex_update.unlock();
}
//------------------------------------------------------------------------------
void		Client::start()
{
	string		name, password;

	cout << "(If you want to sign up, type 'signup')\n";
	cout << "name: ";
	cout.flush();
	cin >> name;

	if (name == "signup")
	{
		status = Status::SIGN_UP;
		return;
	}

	cout << "password: ";
	cin >> password;

	// ToDo: check name validate

	Message		message;
	message.set_length<ReqConnect>();
	message.type = MessageType::REQ_LOG_IN;
	memset(&message.get_body<ReqConnect>(), 0, message.get_body_length());
	memcpy(&message.get_body<ReqConnect>().name, name.data(), name.length());
	memcpy(&message.get_body<ReqConnect>().password, password.data(), password.length());
	
	socket_send(message);
	status = Status::LOGIN_WAIT;
}
//------------------------------------------------------------------------------
void		Client::sign_up()
{
	string		name, password;
	cout << "new name: ";
	cout.flush();
	cin >> name;
	cout << "password: ";
	cin >> password;

	Message		message;
	message.set_length<ReqConnect>();
	message.type = MessageType::REQ_SIGN_UP;
	memset(&message.get_body<ReqConnect>(), 0, message.get_body_length());
	memcpy(&message.get_body<ReqConnect>().name, name.data(), name.length());
	memcpy(&message.get_body<ReqConnect>().password, password.data(), password.length());
	
	socket_send(message);
	status = Status::SIGN_UP_WAIT;
}
//------------------------------------------------------------------------------
void		Client::dead()
{
	KeyManager	k_exit0('Q');
	KeyManager	k_exit1(VK_ESCAPE);
	cout << "You DIED!" << endl;
	cout << "Press ESC or Q to Quit" << endl;
	while (1)
	{
		if (k_exit0.is_pressed() || k_exit1.is_pressed())
		{
			status = Status::EXIT;
			return;
		}
	}
}
//------------------------------------------------------------------------------
void		Client::connect_wait()
{
	cout << "Connecting..." << endl;
	Sleep(100);
}

//------------------------------------------------------------------------------
double			Client::time_now()
{
	return static_cast<double>(chrono::duration_cast<chrono::microseconds>
		(clock::now() - time_start).count()) / 1000;
}
//------------------------------------------------------------------------------
void			Client::operator()(PrintScreen dummy)
{
	screen.clear_buffer();
	for (auto& bullet : my_bullets)
	{
		screen.draw_point(
			bullet.second.movable.position.x,
			bullet.second.movable.position.y,
			'*');
	}

	// critical section
	{
		lock_guard<mutex>	g(mutex_update);

		for (auto& player : players)
		{
			screen.draw_point(
				player.second.movable.position.x,
				player.second.movable.position.y,
				player.second.shape);
			// cout << player.second.name << endl;
		}
		for (auto& bullet : bullets)
		{
			screen.draw_point(
				bullet.second.movable.position.x,
				bullet.second.movable.position.y,
				'*');
		}
	}
	screen.draw_point(me.movable.position.x, me.movable.position.y, me.shape);
	screen.print();
	cout << "name: " << me.name 
	<< "dir: " << me.movable.direction 
	<< "pos: " << me.movable.position << endl;
}
//------------------------------------------------------------------------------
void			Client::operator()(SendMessage dummy)
{

	Message		message;
	message.type = MessageType::REQ_GAME;
	message.set_length<ReqGame, BulletBase>(my_bullets.size());
	ReqGame&	req_game = message.get_body<ReqGame>();
	BulletBase*	s_bullets = &message.get_body<ReqGame, BulletBase>();
	req_game.player = me;
	
	lock_guard<mutex>	g(mutex_update);
	
	req_game.len_bullets = my_bullets.size();
	int		i = 0;
	for (auto it : my_bullets)
	{
		s_bullets[i++] = it.second;
	}
	socket_send(message);
}
//------------------------------------------------------------------------------
bool			Client::set_player(PlayerBase* r_player)
{
	auto	it = players.find(r_player->id);
	if (it == players.end())
		return false;
	PlayerBase&	player = it->second;
	player.time_recv = time_now();
	return true;
}
//------------------------------------------------------------------------------
void			Client::socket_send(Message& message)
{
	message.set_hash();
	socket.send(&message, message.length);
}