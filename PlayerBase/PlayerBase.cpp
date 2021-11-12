#include "PlayerBase.hpp"

PlayerBase::PlayerBase(const Movable& movable, uint32_t id, std::string name, char shape)
:	movable(movable),
	id(id),
	name(name),
	shape(shape)
{}

ResGamePlayer		PlayerBase::cast_response()
{
	return ResGamePlayer {movable.cast_movedata(), id, shape};
}