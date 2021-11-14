#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>

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
		DEAD,
		EXIT
	};
	
	SocketClient					socket;

	std::unordered_map<uint32_t, PlayerBase>	players;
	PlayerBase						me;
	
	std::unordered_map<uint32_t, BulletBase>	bullets;
	std::unordered_map<uint32_t, BulletBase>	my_bullets;

	Status							status;
	clock::time_point				time_start;

	Screen							screen;
	std::mutex						mutex_update;

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
	void		service_connect(Message& message);
	void		service_game(Message& message);

	//	status			-----------------
	void		start();
	void		login_wait();
	void		game();
	void		dead();
	void		shot(uint32_t& bullet_count);
	void		update(double time);

	//	etc				-----------------
	double		time_now();
	struct		PrintScreen{};
	void		operator()(PrintScreen);
	struct		SendMessage{};
	void		operator()(SendMessage);

	bool		set_player(PlayerBase* r_player);

	void		socket_send(Message& message);
};
//------------------------------------------------------------------------------
