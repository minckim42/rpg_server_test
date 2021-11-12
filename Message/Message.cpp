#include "Message.hpp"

uint32_t		Message::get_body_length()
{
	return length - offsetof(Message, body);
}