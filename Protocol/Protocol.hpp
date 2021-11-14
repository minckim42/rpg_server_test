#pragma once
#include "../Message/Message.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace MessageType {
	enum Type
	{
		REQ_GAME,
		REQ_LOG_IN,
		REQ_SIGN_UP,

		RES_GAME,
		RES_CONNECT,
		RES_LOG_IN_FAIL,
		RES_SIGN_UP_FAIL,
	};
}