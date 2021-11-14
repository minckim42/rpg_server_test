#pragma once
#include "PlayerBase.hpp"
#include <stdint.h>
#pragma pack(1)

struct ResGame
{
	uint32_t	len_players;
	uint32_t	len_bullets;
};

struct ResConnect
{
	PlayerBase		player;
};

struct ResFail
{
	uint32_t		dummy;
};

#pragma pack()