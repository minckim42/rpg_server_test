#include <iostream>
#include <iomanip>
#include "KeyManager.hpp"

using namespace std;

// 픽셀 공간
class screen
{
public:
	int				width;
	int				height;
	//vector<string>	pixel;
	char*			pixel;
	char*			cleared;
	//------------------------------------------------------------------------------
	screen(int width, int height)
	: width(width), height(height) //, pixel(height, string(width, ' '))
	{
		pixel = new char[(width + 1) * height + 1];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				pixel[(width + 1) * y + x] = ' ';
			}
			pixel[(width + 1) * y + width] = '\n';
		}
		pixel[(width + 1) * (height - 1)] = 0;
		// 빈 스크린
		cleared = new char[(width + 1) * height + 1];
		memcpy(cleared, pixel, sizeof(char) * ((width + 1) * height + 1));
	}
	//------------------------------------------------------------------------------
	~screen()
	{
		delete[] pixel;
		delete[] cleared;
	};
	//------------------------------------------------------------------------------
	// 0,0은 좌측 하단인데, 출력버퍼는 좌측 상단에서 시작하므로, 좌표계 변환기가 필요하다.
	inline
	char&		get_pixel(int x, int y)
	{
		return pixel[(width + 1) * (height - 2 - y) + x];
	}
	//------------------------------------------------------------------------------
	void		clear()
	{
		memcpy(pixel, cleared, sizeof(char) * ((width + 1) * height + 1));
	}
	//------------------------------------------------------------------------------
	// 커서를 0,0으로 옮기고 출력한다.
	void		print()
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
		cout << pixel;
	}
private:
};
//------------------------------------------------------------------------------
// 2차원 벡터
struct vec2
{
	float	x;
	float	y;
	vec2() : x(0), y(0) {}
	vec2(float x, float y) : x(x), y(y)		{}

	vec2	operator+(const vec2& other)	{ return vec2(x + other.x, y + other.y); }
	vec2	operator-(const vec2& other)	{ return vec2(x - other.x, y - other.y); }
	float	operator*(const vec2& other)	{ return x * other.x + y * other.y; }
	vec2	operator*(float other)			{ return vec2(x * other, y * other); }
	
	vec2&	operator+=(const vec2& other)	{ x += other.x; y += other.y; return *this; }
	vec2&	operator-=(const vec2& other)	{ x -= other.x; y -= other.y; return *this; }
	vec2&	operator*=(float other)			{ x *= other; y *= other; return *this; }

	void	rot(float rad)
	{
		x = x * cosf(rad) - y * sinf(rad);
		y = x * sinf(rad) + y * cosf(rad);
	}
};
//------------------------------------------------------------------------------
ostream& operator<<(ostream& os, vec2& v)
{
	os	<< setw(10) << setprecision(6) << v.x << ", " 
		<< setw(10) << setprecision(6) << v.y;
	return os;
}
//------------------------------------------------------------------------------
template <typename T>
T		clamp(T mn, T mx, T v)
{
	return min(mx, max(mn, v));
}
//------------------------------------------------------------------------------
template <typename T>
bool	is_in_range(T mn, T mx, T v)
{
	return mn <= v && v < mx;
}
//------------------------------------------------------------------------------
class character
{
public:
	typedef std::chrono::high_resolution_clock::time_point	time_point;
	typedef std::chrono::duration<double>					duration;

public:
	vec2		position;
	vec2		velocity;
	vec2		acceleration;
	float		friction;

	float		velocity_normal;
	float		velocity_dash;
	float		velocity_jump;
	float		gravity;

	double		interval;
	time_point	prev;
	//------------------------------------------------------------------------------
	character() :
		friction(1),
		velocity_normal(15),
		velocity_dash(40),
		velocity_jump(40),
		gravity(5),
		interval(0),
		prev(chrono::high_resolution_clock::now())
	{}
	//------------------------------------------------------------------------------
	void	move_left()
	{
		if (position.y == 0)
			acceleration += vec2(-velocity_normal, 0);
	}
	//------------------------------------------------------------------------------
	void	move_right()
	{
		if (position.y == 0)
			acceleration += vec2(velocity_normal, 0);
	}
	//------------------------------------------------------------------------------
	void	jump()
	{
		if (position.y == 0)
			velocity += vec2(0, velocity_jump);
	}
	//------------------------------------------------------------------------------
	void	dash_left()
	{
		velocity += vec2(-velocity_dash, 0);
	}
	//------------------------------------------------------------------------------
	void	dash_right()
	{
		velocity += vec2(velocity_dash, 0);
	}
	//------------------------------------------------------------------------------
	void	update()
	{
		time_point		current = chrono::high_resolution_clock::now();

		duration	interval_duration = current - prev;
		interval = interval_duration.count();

		if (position.y == 0)
			acceleration -= velocity * friction;
		else
			acceleration.y -= gravity;
		velocity += acceleration * interval;
		position += velocity * interval;

		acceleration.x = 0;
		position.y = max(0, position.y);
		if (position.y == 0)
		{
			acceleration.y = 0;
			velocity.y = 0;
		}
		prev = current;
	}
	//------------------------------------------------------------------------------
	void	print(screen& s)
	{
		if (is_in_range(0, s.width, (int)position.x) && is_in_range(0, s.height, (int)position.y))
			s.get_pixel(position.x, position.y) = 'a';
	}
};
//------------------------------------------------------------------------------
// 동작
void	jump(int key, void* arg)
{
	character* c = static_cast<character*>(arg);
	c->jump();
}
//------------------------------------------------------------------------------
void	move_right(int key, void* arg)
{
	character* c = static_cast<character*>(arg);
	c->move_right();
}
//------------------------------------------------------------------------------
void	move_left(int key, void* arg)
{
	character* c = static_cast<character*>(arg);
	c->move_left();
}
//------------------------------------------------------------------------------
void	dash_left(int key, void* arg)
{
	character* c = static_cast<character*>(arg);
	c->dash_left();
}
//------------------------------------------------------------------------------
void	dash_right(int key, void* arg)
{
	character* c = static_cast<character*>(arg);
	c->dash_right();
}
//------------------------------------------------------------------------------
void	say_bye(int key, int other_key)
{
	cout << "bye" << endl;
	exit(0);
}
//------------------------------------------------------------------------------
int		main(void)
{
	KeyManager					key_jump(' ');
	KeyManager					key_move_l(VK_LEFT);
	KeyManager					key_move_r(VK_RIGHT);
	KeyManager					key_exit('Q');

	screen						s(120, 27);
	character					c;
	double						interval_avg = 0;

	while (1)
	{
		key_jump.is_pressed(jump, &c);
		key_move_l.is_pressed(move_left, &c);
		key_move_r.is_pressed(move_right, &c);
		key_move_l.is_pressed_double(dash_left, &c);
		key_move_r.is_pressed_double(dash_right, &c);
		key_exit.is_pressed_comb(say_bye, VK_LCONTROL);
		
		s.clear();
		c.update();
		c.print(s);
		s.print();
		Sleep(1);
		cout << "position: " << c.position
			<< " | velocity: " << c.velocity
			<< " | acceleration: " << c.acceleration << endl;
		interval_avg = interval_avg * 0.9 + c.interval * 0.1;
		cout << "interval: " 
			<< setw(10) << setprecision(6) << interval_avg 
			<< "  fps: " 
			<< setw(10) << setprecision(6) << 1 / interval_avg << endl;
	}
}