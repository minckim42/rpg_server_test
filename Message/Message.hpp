#pragma once
#include <stdint.h>
#include <stddef.h>
#include <random>
#pragma pack(1)
/*##############################################################################

	Message

##############################################################################*/
struct Message
{
	/*---------------------------------
		Header
	---------------------------------*/	
	uint32_t	length;
	uint32_t	type;
	uint32_t	seed;
	uint32_t	hash;
	/*---------------------------------
		body
	---------------------------------*/	
	char		body[4096];

	/*---------------------------------
		Methods
	---------------------------------*/	
	template <typename T>
	T&			get_body()
	{
		return reinterpret_cast<T&>(*body);
	}
	//--------------------------------------------------------------------------
	template <typename T, typename A>
	A&			get_body()
	{
		return *reinterpret_cast<A*>(body + sizeof(T));
	}
	//--------------------------------------------------------------------------
	template <typename T, typename A, typename B>
	B&			get_body(int a)
	{
		return reinterpret_cast<B&>(*(body + sizeof(T) + sizeof(A) * a));
	}
	//--------------------------------------------------------------------------
	template <typename T>
	void		set_length(int additional = 0)
	{
		length = offsetof(Message, body) + sizeof(T) + additional;
	}
	//--------------------------------------------------------------------------
	template <typename T, typename A>
	void		set_length(int a)
	{
		length = offsetof(Message, body) + sizeof(T) + sizeof(A) * a;
	}
	//--------------------------------------------------------------------------
	template <typename T, typename A, typename B>
	void		set_length(int a, int b)
	{
		length = offsetof(Message, body) + sizeof(T) + sizeof(A) * a + sizeof(B) * b;
	}
	//--------------------------------------------------------------------------
	uint32_t	get_body_length();
	uint32_t	get_hash();
	uint32_t	get_hash(uint32_t size);
	void		set_hash();
	bool		check_hash();
};
#pragma pack()

/*---------------------------------
	Non member
---------------------------------*/	
uint32_t		easy_random(uint32_t a, uint32_t b);