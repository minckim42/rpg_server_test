#pragma once
#include "Movable.hpp"
#include <stdint.h>
#pragma pack(1)

struct ResPosition
{
	uint32_t	len_players;
	uint32_t	len_bullets;
};

struct ResPositionPlayer
{
	Movable		status;
	uint32_t	id;
};

struct ResPositionBullet
{
	Movable		status;
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
	uint32_t	result;
};

#pragma pack()