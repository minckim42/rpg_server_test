#include "Message.hpp"

uint32_t		Message::get_body_length()
{
	return length - offsetof(Message, body);
}
//--------------------------------------------------------------------------
uint32_t		Message::get_hash()
{
	return get_hash(get_body_length());
}
//--------------------------------------------------------------------------
uint32_t		Message::get_hash(uint32_t size)
{
	uint32_t	tmp = 0;
	for (uint32_t i = 0 ; i < size ; i++)
	{
		tmp *= body[i] * i + seed * i;
	}
	return tmp;
}
//--------------------------------------------------------------------------
void			Message::set_hash()
{
	seed = easy_random(0, 0xffffffff);
	hash = get_hash();
}
//--------------------------------------------------------------------------
bool			Message::check_hash()
{
	return get_hash() == hash;
}
//--------------------------------------------------------------------------
uint32_t		easy_random(uint32_t a, uint32_t b)
{
	static std::random_device		random_device;
	static std::mt19937				generator(random_device());
	static std::uniform_int_distribution<uint32_t>	dis(a, b);
	return dis(generator);
}