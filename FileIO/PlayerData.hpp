#pragma once
#include "../FileIO/FileIO.hpp"

#define CLIENT_PORT 9001
#define SERVER_PORT 9000
#define SERVER_ADDR "127.0.0.1"

#pragma pack(1)
//------------------------------------------------------------------------------
struct		PlayerData
{
	float	position[2];
	char	shape;
	char	name[20];
	float	speed;

	PlayerData();
	void	load();
	void	save();
	void	set_shape(FileData& data);
	void	set_position(FileData& data);
	void	set_name(FileData& data);
	void	set_velocity(FileData& data);
};
//------------------------------------------------------------------------------
struct ConnectRequest
{
	enum Type {
		LOGIN=123,
		LOGOUT,
	};
	uint32_t	value;
};
//------------------------------------------------------------------------------
struct ConnectResponse
{
	enum Type {
		LOGIN_OK,
		LOGOUT_OK,
		FAIL,
	};
	uint32_t	value;
};
//------------------------------------------------------------------------------
struct Instruction
{
	enum InstructionType {
		UP=789,
		DOWN,
		LEFT,
		RIGHT,
		INIT,
	};
	uint32_t	instruction;
};
//------------------------------------------------------------------------------
struct Message
{
	uint32_t	seed;
	uint32_t	hash;
	uint32_t	type;
	char		body[200];

	enum MessageTypes{
		CONNECT_REQUEST,
		CONNECT_RESPONSE,
		INSTRUCTION,
		PLAYER_DATA,
	};

	uint32_t	get_hash();
	void		set_hash();
	bool		check_hash();
	int			size();
	int			body_size();

	template <typename T>
	T&		get_body()
	{
		return reinterpret_cast<T&>(*body);
	}

	template <typename T>
	int		size()
	{
		return offsetof(Message, body) + sizeof(T);
	}

	template <typename T>
	uint32_t	get_hash()
	{
		uint32_t	tmp = 0;
		uint32_t	len_data = size<T>();
		for (uint32_t i = 0 ; i < len_data ; i++)
		{
			tmp *= body[i] * i + seed * i;
		}
		return tmp;			
	}

	template <typename T>
	void		set_hash()
	{
		seed = 293847; // need random
		hash = get_hash<T>();
	}

	template <typename T>
	bool		check_hash()
	{
		return get_hash<T>() == hash;
	}
};
//------------------------------------------------------------------------------
#pragma pack()
