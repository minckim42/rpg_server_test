#include "Movable.hpp"

Movable::Movable(const Vec2& position, const Vec2& direction, float speed)
:	position(position),
	direction(direction),
	speed(speed)
{}

Movable&	Movable::operator=(const Movable& x)
{
	position = Vec2(x.position);
	direction = Vec2(x.direction);
	speed = x.speed;
	return *this;
}

void	Movable::move(double ms)
{
	position += Vec2(direction) * ms;
}

void	Movable::rotate(double radian)
{
	direction.rot(radian);
}

bool	Movable::is_collide(const Movable& x, double ms, float collision_limit)
{
	Vec2	relative = Vec2(x.position) - position;
	float	a = direction * direction;
	float	b = -2 * (relative * direction);
	float	c = relative * relative - collision_limit * collision_limit;

	float	bb4ac = b * b - 4 * a * c;
	
	if (bb4ac < 0)
		return false;

	// float	time_collision0 = (-b + sqrtf(bb4ac)) / (2 * a);
	float	time_collision = (-b - sqrtf(bb4ac)) / (2 * a) / speed;

	if (time_collision > ms)
		return false;
	return true;
}