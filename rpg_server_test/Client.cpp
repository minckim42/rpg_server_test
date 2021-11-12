#include "Client.hpp"
#include <iostream>
using namespace std;
/*##############################################################################

	Client

##############################################################################*/
Client::Client(const string& ip, int port)
:	socket(ip, port), status(Status::START)
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
			login_wait();
			break;

		case Status::GAME:
			game();
			break;
		
		default:
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
	// Message			message;
	// ReqGame*		game_message = &message.get_body<ReqGame>();
	// ReqGameBullet*	bullet0 = &game_message->bullet;
	// ReqGameBullet*	bullet1 = bullet0 + 1;

	// game_message->id = 3;
	// game_message->move_status = {8, 1, 0, 0, 1};
	// game_message->len_bullets = 2;
	// *bullet0 = {1001, {1, 1, 1, 0, 1}, 3};
	// *bullet0 = {1002, {1, 1, 1, 0, 1}, 4};
	
	// socket.send(&message, message.size<ReqGame>() + 2 * sizeof(ReqGameBullet));
}
//------------------------------------------------------------------------------
void		Client::service_connect(ResConnect& res)
{
	cout << "result: " << res.result << endl;
	cout << "user id: " << res.user_id << endl;
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
}