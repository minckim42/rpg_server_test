#include "Movable.hpp"
#include <iostream>
int		main()
{
	Movable		a;
	Movable		b;

	a.position = Vec2(1, 2);
	a.direction = Vec2(3, 2);
	a.direction = a.direction.unit();
	a.speed = 1;
	b.position = Vec2(4, 3);
	std::cout << a.is_collide(b, 1, 1) << std::endl;
	std::cout << a.is_collide(b, 2, 1) << std::endl;
	std::cout << a.is_collide(b, 3, 1) << std::endl;
	std::cout << a.is_collide(b, 4, 1) << std::endl;
}