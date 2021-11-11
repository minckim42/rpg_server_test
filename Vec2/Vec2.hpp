#include <math.h>
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
