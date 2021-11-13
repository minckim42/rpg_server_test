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
	cout << this << endl;
	while (1)
	{
		switch (status)
		{
		case Status::START:
			start();
			break;

		case Status::LOGIN_WAIT:
			login_wait();
			break;

		case Status::GAME:
			game();
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
			service_connect(message.get_body<ResConnect>());
			break;

		case MessageType::RES_GAME:
			service_game(message.get_body<ResGame>());
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

	FrameManager	screen_manager(100);

	double		time0 = time_now();
	uint32_t	bullet_count = 0;
	while (1)
	{
		double	time_cur = time_now();

		me->movable.speed = 0;
		// instruction
		double	time1 = time_now();
		if (k_turn_left.is_pressed())
			me->movable.rotate((time_cur - time0) / 1000);
		if (k_turn_right.is_pressed())
			me->movable.rotate(-(time_cur - time0) / 1000);
		if (k_move.is_pressed())
			me->movable.speed = me->speed;
		if (k_shot.is_pressed())
		{
			shot(bullet_count);
		}
		if (k_exit0.is_pressed() || k_exit1.is_pressed())
		{
			status = Status::EXIT;
			return;
		}

		update(time_cur - time0);
		time0 = time_cur;



		screen_manager.set_interval(*this, PrintScreen());
	}
}
//------------------------------------------------------------------------------
void		Client::shot(uint32_t bullet_count)
{
	// uint32_t	bullet_id = me->id * 0x1000 + bullet_count++;
	// bullets[bullet_id] = BulletBase();
	// BulletBase*	ptr_bullet = &bullets[bullet_id];
	// my_bullets.push_back(ptr_bullet);
	// ptr_bullet->id = bullet_id;
	// ptr_bullet->movable = me->movable;
	// ptr_bullet->movable.speed = SPEED_BULLET;
}
//------------------------------------------------------------------------------
void		Client::update(double time)
{
	// vector<BulletBase*>		to_be_erased;
	// to_be_erased.reserve(my_bullets.size());

	// for (BulletBase* ptr_bullet : my_bullets)
	// {
	// 	for (pair<const uint32_t, PlayerBase>& player_pair : players)
	// 	{
	// 		PlayerBase&	player = player_pair.second;
	// 		if (&player == me)
	// 			continue;
	// 		if (ptr_bullet->movable.is_collide(player.movable, time, 1))
	// 		{
	// 			to_be_erased.push_back(ptr_bullet);
	// 			player.id_alive = false;
	// 		}
	// 	}
	// }

	for (pair<const uint32_t, PlayerBase>& player_pair : players)
	{
		PlayerBase&	player = player_pair.second;
		player.movable.update(time);
	}
}

//------------------------------------------------------------------------------
void		Client::service_connect(ResConnect& res)
{
	if (res.result == ResConnect::LOGIN_SUCCESS)
	{
		players[res.player.id] = res.player;
		me = &players[res.player.id];
		
		cout << "Login Success" << endl;
		cout << "ID: " << res.player.id << endl;
		cout << "shape: " << res.player.shape << endl;
		cout << "name: " << res.player.name << endl;
		
		status = Status::GAME;
	}
	else
	{
		cout << "Login Failed" << endl;
		status = Status::START;
	}
}
//------------------------------------------------------------------------------
void		Client::service_game(ResGame& res)
{

}

void		Client::start()
{
	string		name, password;

	cout << "name: ";
	cout.flush();
	cin >> name;
	cout << "password: ";
	cin >> password;

	// ToDo: check name validate

	Message		message;
	message.set_length<ReqLogin>();
	message.type = MessageType::REQ_LOGIN;
	memset(&message.get_body<ReqLogin>(), 0, message.get_body_length());
	memcpy(&message.get_body<ReqLogin>().name, name.data(), name.length());
	memcpy(&message.get_body<ReqLogin>().password, password.data(), password.length());
	
	socket.send(&message, message.length);
	status = Status::LOGIN_WAIT;

}

void		Client::login_wait()
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
	for (pair<const uint32_t, PlayerBase>& player_pair : players)
	{
		PlayerBase&	player = player_pair.second;
		screen.draw_point(player.movable.position.x,
			player.movable.position.y,
			player.shape);
	}
	for (pair<const uint32_t, BulletBase>& bullet_pair : bullets)
	{
		BulletBase&	bullet = bullet_pair.second;
		screen.draw_point(bullet.movable.position.x,
			bullet.movable.position.y,
			'*');
	}
	screen.print();
	cout << "direction: " << me->movable.direction << endl;
}