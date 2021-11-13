#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include "Database.hpp"

#include <unordered_map>
#include <iostream>
#include <chrono>
#include <mutex>

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

	Server(int port);

	struct		Accept{};
	void		operator()(Accept);
	struct		Controller{};
	void		operator()(Controller);

	void		controller(SocketEndpoint* endpoint);

	void		service_login(SocketEndpoint* endpoint);
	void		service_disconnect(SocketEndpoint* endpoint);
	void		service_game(SocketEndpoint* endpoint);


	LocalData	load_player_data(
					const std::string& name, 
					const std::string& password);
	PlayerBase	init_player(Database::SelectData& data);


	Message&	get_recv_message(SocketEndpoint* endpoint);
	Message&	get_send_message(SocketEndpoint* endpoint);
	double		time_now();
	void		print_log(const std::string& str);

	bool		set_player(PlayerBase* r_player);
	bool		set_player_dead(uint32_t key);
	void		set_bullet(BulletBase* r_bullet);
};