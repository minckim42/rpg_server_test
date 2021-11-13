#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include <vector>
#include <map>
#include <string>
/*##############################################################################

	Client

##############################################################################*/
class Client
{
	/*---------------------------------
		Member
	---------------------------------*/	
	SocketClient			socket;

	std::map<uint32_t, PlayerBase>	players;
	std::map<uint32_t, BulletBase>	bullets;
	PlayerBase*						me;
	std::vector<BulletBase*>		my_bullets;

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

	//	etc				-----------------
};
//------------------------------------------------------------------------------
