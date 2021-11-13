#pragma once
#include "Movable.hpp"
#include <string>
#define LEN_NAME	16

#pragma pack(1)

struct PlayerBase
{
	/*---------------------------------
		Member
	---------------------------------*/
	public:

	Movable			movable;
	uint32_t		id;
	bool			id_alive;
	bool			is_moving;
	double			time_recv;
	double			time_send;
	char			shape;
	char			name[LEN_NAME];
	float			speed;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	public:
	// PlayerBase()=default;
	// PlayerBase(const Movable& movable, uint32_t id, const std::string& name, char shape);
};

#define SPEED_BULLET	0.01;

struct BulletBase
{
	Movable			movable;
	double			time_birth;
	double			time_recv;
	double			time_send;
	uint32_t		id;
};

#pragma pack()