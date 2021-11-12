#pragma once
#include "../Movable/Movable.hpp"
#include "../Socket/SocketClient.hpp"
#include "../Protocol/Protocol.hpp"
#include <vector>
#include <map>
#include <string>
/*##############################################################################

	Client

##############################################################################*/
class Player
{
	Movable		movable;
};
//------------------------------------------------------------------------------
class Bullet
{
	Movable		movable;
};
//------------------------------------------------------------------------------
class Client
{
	/*---------------------------------
		Member
	---------------------------------*/	
	SocketClient			socket;

	std::vector<Player>		players;
	std::vector<Bullet>		bullets;

	enum class Status
	{
		STATUS_START,
		STATUS_GAME
	};
	Status					status;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/	
	public:
	Client(const std::string& ip, int port);

	/*---------------------------------
		Methods
	---------------------------------*/

	void		run();

	//	thread routine	-----------------
	struct		Controller{};
	void		operator()(Controller);

	//	Controller		-----------------
	void		controller(Message& message);

	//	Service			-----------------
	void		service_game(ResGame& res_game);

	//	status			-----------------
	void		start();
	void		game();
};
//------------------------------------------------------------------------------
