#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include "../Database/Database.hpp"

#include <unordered_map>
#include <iostream>
#include <chrono>
#include <mutex>
#include <sstream>

#define __DATABASE

class Server
{
	public:
	typedef		std::chrono::system_clock		clock;

	#ifdef __DATABASE
	typedef		Database::SelectData			LocalData;
	#endif

	SocketIOCP									socket;
	std::unordered_map<uint32_t, PlayerBase>	players;
	std::unordered_map<uint32_t, BulletBase>	bullets;
	std::unordered_map<SOCKET, uint32_t>		sock_id_map;
	Database									database;
	clock::time_point							time_start;

	std::mutex									mutex_players;
	std::mutex									mutex_bullets;
	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/	
	Server(int port);

	/*---------------------------------
		Methods
	---------------------------------*/

	//	thread routine	-----------------
	struct		Accept{};
	void		operator()(Accept);
	struct		Controller{};
	void		operator()(Controller);

	//	Controller		-----------------
	void		controller(SocketEndpoint* endpoint);

	//	Service			-----------------
	void		service_log_in(SocketEndpoint* endpoint);
	void		service_log_in_fail(SocketEndpoint* endpoint);
	void		service_sign_up(SocketEndpoint* endpoint);
	void		service_sign_up_fail(SocketEndpoint* endpoint);
	void		service_disconnect(SocketEndpoint* endpoint);
	void		service_game(SocketEndpoint* endpoint);

	//	etc				-----------------
	void		save_data(uint32_t id);
	LocalData	load_player_data(
					const std::string& name, 
					const std::string& password);
	PlayerBase	init_player(Database::SelectData& data);
	void		create_new_user(const std::string& name, const std::string& password);

	bool		set_player(PlayerBase* r_player);
	void		set_bullet(BulletBase* r_bullet);
	bool		set_player_dead(uint32_t key);
	uint32_t	find_user_name(const std::string& name);

	double		time_now();
	void		print_log(const std::string& str);
	Message&	get_recv_message(SocketEndpoint* endpoint);
	Message&	get_send_message(SocketEndpoint* endpoint);
};