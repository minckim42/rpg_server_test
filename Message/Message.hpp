#pragma once
#include <stdint.h>
#include <stddef.h>
#pragma pack(1)

struct Message
{
	uint32_t	length;
	uint32_t	type;
	uint32_t	seed;
	uint32_t	hash;
	char		body[1000];

	template <typename T>
	T&			get_body()
	{
		return reinterpret_cast<T&>(*body);
	}

	template <typename T>
	void		set_length(int additional = 0)
	{
		length = offsetof(Message, body) + sizeof(T) + additional;
	}

	uint32_t	get_body_length();

	template <typename T>
	uint32_t	get_hash()
	{
		return get_hash(get_body_length());
	}

	uint32_t	get_hash(uint32_t size)
	{
		uint32_t	tmp = 0;
		for (uint32_t i = 0 ; i < size ; i++)
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