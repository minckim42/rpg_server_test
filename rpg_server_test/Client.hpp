#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>

/*##############################################################################

	Client

##############################################################################*/

#define SCREEN_SIZE		100,27

class Client
{
	public:
	typedef std::chrono::system_clock	clock;
	/*---------------------------------
		Member
	---------------------------------*/	
	enum class Status
	{
		START,
		LOGIN_WAIT,
		GAME,
		EXIT
	};
	
	SocketClient					socket;

	std::map<uint32_t, PlayerBase>	players;
	std::map<uint32_t, BulletBase>	bullets;
	PlayerBase*						me;
	std::vector<BulletBase*>		my_bullets;

	Status							status;
	clock::time_point				time_start;

	Screen							screen;

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
	void		shot(uint32_t bullet_count);
	void		update(double time);

	//	etc				-----------------
	double		time_now();
	struct		PrintScreen{};
	void		operator()(PrintScreen);
	struct		SendMessage{};
	void		operator()(SendMessage);
};
//------------------------------------------------------------------------------
