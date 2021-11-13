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
	enum	ResultType
	{
		LOGIN_SUCCESS,
		LOGIN_FAIL,
		LOGOUT_SUCCESS,
		LOGOUT_FAIL,
	};
	uint32_t		result;
	PlayerBase		player;
};

#pragma pack()