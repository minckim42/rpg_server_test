#pragma once
#include <math.h>
#include "../Vec2/Vec2.hpp"

class Movable
{
	public:
	Vec2	position;
	Vec2	direction;
	float	speed;			// unit length per ms

	//Movable&	operator=(const MoveData& x);
	void		move(double ms);
	void		rotate(double radian);		//	ccw is positive
	bool		is_collide(const Movable& x, double ms, float collision_limit);
};