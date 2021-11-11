#include "Client.hpp"
#include "../Protocol/Protocol.hpp"
using namespace std;
/*##############################################################################

	Client

##############################################################################*/
Client::Client(const string& ip, int port)
:	socket(ip, port)
{}
//------------------------------------------------------------------------------
void		Client::operator()(Client::Run dummy)
{
	while (1)
	{
		switch (status)
		{
		case Status::STATUS_START:
			start();
			break;

		case Status::STATUS_GAME:
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
	}
}
//------------------------------------------------------------------------------
void		Client::controller(Message& message)
{
	switch (message.type)
	{
		case Message::REQ_GAME:
			service_game(message.get_body<ResGame>());
			break;
	}
}
//------------------------------------------------------------------------------
void		Client::game()
{
	Message			message;
	ReqGame*		game_message = &message.get_body<ReqGame>();
	ReqGameBullet*	bullet0 = &game_message->bullet;
	ReqGameBullet*	bullet1 = bullet0 + 1;

	game_message->id = 3;
	game_message->move_status = {8, 1, 0, 0, 1};
	game_message->len_bullets = 2;
	*bullet0 = {1001, {1, 1, 1, 0, 1}, 3};
	*bullet0 = {1002, {1, 1, 1, 0, 1}, 4};
	
	socket.send(&message, message.size<ReqGame>() + 2 * sizeof(ReqGameBullet));
}

