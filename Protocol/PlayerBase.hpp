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
	bool			is_alive;
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

#define BULLET_SPEED	0.04
#define BULLET_LIFE		1000
#define BULLET_COOLDOWN	500

struct BulletBase
{
	Movable			movable;
	uint32_t		id;
	uint32_t		id_hit;
	double			time_birth;
	double			time_recv;
	double			time_send;
};

#pragma pack()