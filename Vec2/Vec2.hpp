#pragma once
#include <math.h>
#include <ostream>
/*##############################################################################

	Vec2

##############################################################################*/
struct Vec2
{
	/*---------------------------------
		Members
	---------------------------------*/
	float	x;
	float	y;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	Vec2();
	Vec2(float x, float y);
	Vec2(const float* ptr);
	Vec2(float* ptr);
	Vec2(const Vec2& other);
	virtual	~Vec2()=default;

	/*---------------------------------
		Methods
	---------------------------------*/
	Vec2	operator+(Vec2 other);
	Vec2	operator-(Vec2 other);
	float	operator*(Vec2 other);
	Vec2	operator*(float other);
	Vec2	operator/(float other);
	Vec2&	operator+=(Vec2 other);
	Vec2&	operator-=(Vec2 other);
	Vec2&	operator*=(float other);
	Vec2&	operator/=(float other);
	void	rot(float rad);
	float	pow();
	float	abs();
	Vec2	unit();
	Vec2	proj(Vec2 other);
	Vec2	perp(Vec2 other);
};
/*---------------------------------
	Nonmember Methods
---------------------------------*/
std::ostream&	operator<<(std::ostream& os, const Vec2& v);