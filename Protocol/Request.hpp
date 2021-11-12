#pragma once
#include "MoveData.hpp"
#include <stdint.h>

#pragma pack(1)

struct ReqGameBullet
{
	uint32_t	id;
	MoveData	move_status;
	uint32_t	id_hit;
};

struct ReqGame
{
	uint32_t		id;
	MoveData		move_status;
	uint32_t		len_bullets;
	ReqGameBullet	bullet;
};

struct ReqLogin
{
	char		name[16];
	char		password[16];
};

struct ReqLogout
{
	uint32_t	dummy;
};

#pragma pack()