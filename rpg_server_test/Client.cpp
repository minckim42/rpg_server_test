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
	Message		message;


	while (1)
	{
		// instruction

		// update

		// send message
		// screen
	}
}
//------------------------------------------------------------------------------
void		Client::service_connect(ResConnect& res)
{
	if (res.result == ResConnect::LOGIN_SUCCESS)
	{
		cout << "Login Success" << endl;
		cout << "ID: " << res.player.id << endl;
		cout << "shape: " << res.player.shape << endl;
		
		players[res.player.id] = res.player;
		me = &players[res.player.id];
		
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