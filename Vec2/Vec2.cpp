#include "Vec2.hpp"
/*##############################################################################

	Vec2

##############################################################################*/
/*---------------------------------
	Constructor & Destructor
---------------------------------*/
Vec2::Vec2()
:	x(0), y(0)
{}
//------------------------------------------------------------------------------
Vec2::Vec2(float x, float y)
:	x(x), y(y)
{}
//------------------------------------------------------------------------------
Vec2::Vec2(const float* ptr)
:	x(ptr[0]), y(ptr[1])
{}
//------------------------------------------------------------------------------
Vec2::Vec2(float* ptr)
:	x(ptr[0]), y(ptr[1])
{}
/*---------------------------------
	Methods
---------------------------------*/
Vec2	Vec2::operator+(Vec2 other)
{
	return Vec2(x + other.x, y + other.y);
}
//------------------------------------------------------------------------------
Vec2	Vec2::operator-(Vec2 other)
{
	return Vec2(x - other.x, y - other.y);
}
//------------------------------------------------------------------------------
float	Vec2::operator*(Vec2 other)
{
	return x * other.x + y * other.y;
}
//------------------------------------------------------------------------------
Vec2	Vec2::operator*(float other)		
{
	return Vec2(x * other, y * other);
}
//------------------------------------------------------------------------------
Vec2	Vec2::operator/(float other)		
{
	return Vec2(x / other, y / other);
}
//------------------------------------------------------------------------------
Vec2&	Vec2::operator+=(Vec2 other)
{
	x += other.x; y += other.y; return *this;
}
//------------------------------------------------------------------------------
Vec2&	Vec2::operator-=(Vec2 other)
{
	x -= other.x; y -= other.y; return *this;
}
//------------------------------------------------------------------------------
Vec2&	Vec2::operator*=(float other)		
{
	x *= other; y *= other; return *this;
}
//------------------------------------------------------------------------------
Vec2&	Vec2::operator/=(float other)		
{
	x /= other; y /= other; return *this;
}
//------------------------------------------------------------------------------
void	Vec2::rot(float rad)
{
	float	tmp[2];
	tmp[0] = x;
	tmp[1] = y;
	x = tmp[0] * cosf(rad) - tmp[1] * sinf(rad);
	y = tmp[0] * sinf(rad) + tmp[1] * cosf(rad);
}
//------------------------------------------------------------------------------
float	Vec2::pow()
{
	return x * x + y * y;
}
//------------------------------------------------------------------------------
float	Vec2::abs()
{
	return sqrt(pow());
}
//------------------------------------------------------------------------------
Vec2	Vec2::unit()
{
	return *this / abs();
}
//------------------------------------------------------------------------------
Vec2	Vec2::proj(Vec2 other)
{
	return other * ((*this * other) / this->pow());
}
//------------------------------------------------------------------------------
Vec2	Vec2::perp(Vec2 other)
{
	return proj(other) - other;
}
//------------------------------------------------------------------------------