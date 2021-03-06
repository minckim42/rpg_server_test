#pragma once
#include <math.h>
#include "../Utils/Utils.hpp"

#pragma pack(1)
struct Movable
{
	public:
	Vec2	position;
	Vec2	direction;
	float	speed;			// unit length per ms

	Movable()=default;
	Movable(const Vec2& position, const Vec2& direction, float speed);
	Movable&	operator=(const Movable& x);

	void		update(double ms);
	void		rotate(double radian);		//	ccw is positive
	bool		is_collide(const Movable& x, double ms, float collision_limit);
};
#pragma pack()