#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include "../PlayerBase/PlayerBase.hpp"
#include "Database.hpp"

#include <unordered_map>
#include <iostream>

#define __DATABASE

class Server
{
	public:
	#ifdef __DATABASE
	typedef		Database::SelectData			LocalData;
	#endif

	SocketIOCP									socket;
	std::unordered_map<uint32_t, PlayerBase>	players;
	std::unordered_map<uint32_t, BulletBase>	bullets;
	std::unordered_map<SOCKET, uint32_t>		sock_id_map;

	Database	database;

	Server(int port);

	struct		Accept{};
	void		operator()(Accept);
	struct		Controller{};
	void		operator()(Controller);

	void		controller(SocketEndpoint* endpoint);

	void		service_login(SocketEndpoint* endpoint);
	void		service_disconnect(SocketEndpoint* endpoint);


	LocalData	load_player_data(
					const std::string& name, 
					const std::string& password);
	PlayerBase	init_player(Database::SelectData& data);


	Message&	get_recv_message(SocketEndpoint* endpoint);
	Message&	get_send_message(SocketEndpoint* endpoint);
};
void		print_log(const std::string& str);