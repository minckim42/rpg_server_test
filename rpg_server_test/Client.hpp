#pragma once
#include "../Movable/Movable.hpp"
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include "../PlayerBase/PlayerBase.hpp"
#include <vector>
#include <map>
#include <string>
/*##############################################################################

	Client

##############################################################################*/
class Player: public PlayerBase
{
};
//------------------------------------------------------------------------------
class Bullet: public BulletBase
{
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
		START,
		LOGIN_WAIT,
		GAME
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
	void		service_connect(ResConnect& res);
	void		service_game(ResGame& res);

	//	status			-----------------
	void		start();
	void		login_wait();
	void		game();
};
//------------------------------------------------------------------------------
