#pragma once
#include "../Socket/SocketIOCP.hpp"
#include "../Movable/Movable.hpp"
#include "../Protocol/Protocol.hpp"
#include <unordered_map>

class Player
{
	public:
	Movable		movable;
};

class Bullet
{
	public:
	Movable		movable;
};

class Server
{
	public:
	SocketIOCP								socket;
	std::unordered_map<uint32_t, Player>	players;
	std::unordered_map<uint32_t, Bullet>	bullets;

	Server(int port);

	struct		Accept{};
	void		operator()(Accept);
	struct		Controller{};
	void		operator()(Controller);

	void		controller(Message& message);

};