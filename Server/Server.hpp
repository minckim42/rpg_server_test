#pragma once
#include "../Utils/Utils.hpp"
#include "../Protocol/Protocol.hpp"
#include "../PlayerBase/PlayerBase.hpp"
#include "Database.hpp"

#include <unordered_map>
#include <iostream>


class Server
{
	public:
	SocketIOCP									socket;
	std::unordered_map<uint32_t, PlayerBase>	players;
	std::unordered_map<uint32_t, BulletBase>	bullets;

	Database	database;

	Server(int port);

	struct		Accept{};
	void		operator()(Accept);
	struct		Controller{};
	void		operator()(Controller);

	void		controller(SocketEndpoint* endpoint);

	void		service_login(SocketEndpoint* endpoint);

	Message&	get_recv_message(SocketEndpoint* endpoint);
	Message&	get_send_message(SocketEndpoint* endpoint);
};
void		print_log(const std::string& str);