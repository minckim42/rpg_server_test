#pragma once
#include "MoveData.hpp"
#include <stdint.h>
#pragma pack(1)

struct ResGame
{
	uint32_t	len_players;
	uint32_t	len_bullets;
};

struct ResGamePlayer
{
	MoveData	move_status;
	uint32_t	id;
	char		shape;
};

struct ResGameBullet
{
	MoveData	move_status;
	uint32_t	id_hit;
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
	ResGamePlayer	player;
};

#pragma pack()