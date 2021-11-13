#pragma once
#include "PlayerBase.hpp"
#include <stdint.h>

#pragma pack(1)

struct ReqGame
{
	PlayerBase	player;
	uint32_t	len_bullets;
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