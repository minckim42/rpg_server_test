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
	enum {
		ALIVE,
		DEAD
	};

	Movable			movable;
	uint32_t		id;
	uint32_t		status;
	double			time_recv;
	double			time_send;
	char			shape;
	char			name[LEN_NAME];

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	public:
	// PlayerBase()=default;
	// PlayerBase(const Movable& movable, uint32_t id, const std::string& name, char shape);
};

struct BulletBase
{
	public:
	Movable		movable;
	uint32_t	id;
};

#pragma pack()