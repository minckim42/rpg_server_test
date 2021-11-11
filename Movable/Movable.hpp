#pragma once
#include <math.h>
#include "../Vec2/Vec2.hpp"

#pragma pack(1)
struct Movable
{
	Vec2	position;
	Vec2	direction;
	float	speed;			// unit length per ms

	void	move(double ms);
	void	rotate(double radian);		//	ccw is positive
	bool	is_collide(const Movable& x, double ms, float collision_limit);
};
#pragma pack()