#pragma once
#include "Movable.hpp"
#include <stdint.h>
#pragma pack(1)

struct ReqPosition
{
	Movable		status;
	uint32_t	len_bullets;
};

struct ReqPositionBullet
{
	Movable		status;
	uint32_t	id_hit;
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