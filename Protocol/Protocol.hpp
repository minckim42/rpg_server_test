#pragma once
#include "../Message/Message.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace MessageType {
	enum Type
	{
		REQ_GAME,
		REQ_LOGIN,
		REQ_LOGOUT,

		RES_GAME,
		RES_CONNECT
	};
}