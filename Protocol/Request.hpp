#pragma once
#include "PlayerBase.hpp"
#include <stdint.h>

#pragma pack(1)

struct ReqGame
{
	PlayerBase	player;
	uint32_t	len_bullets;
};

struct ReqConnect
{
	char		name[16];
	char		password[16];
};

#pragma pack()