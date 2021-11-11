#pragma once
#include <stdint.h>
#pragma pack(1)

struct Message
{
	uint32_t	length;
	uint32_t	type;
	uint32_t	seed;
	uint32_t	hash;
	char		body[1000];

	uint32_t	get_hash();
	void		set_hash();
	bool		check_hash();
	int			size();
	int			body_size();

	template <typename T>
	T&			get_body()
	{
		return reinterpret_cast<T&>(*body);
	}

	template <typename T>
	int			size()
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

#pragma pack()