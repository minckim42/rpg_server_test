#include "PlayerBase.hpp"

PlayerBase::PlayerBase(const Movable& movable, uint32_t id, std::string name)
:	movable(movable),
	id(id),
	name(name)
{}