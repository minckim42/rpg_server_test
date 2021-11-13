#include "Screen.hpp"

// #define BASIC_TEST
#define PRINT_STRING

int		main()
{
	Screen	s(100, 50);

#ifdef BASIC_TEST
	while (1)
	{
		s.get_pixel(3, 3) = 'A';
		s.print();
		s.clear();
		Sleep(10);
	}
#endif

#ifdef PRINT_STRING
	while (1)
	{
		s.draw_string(std::string("hello"), 5, 6);
		s.draw_string(std::string("hello"), 98, 5);
		s.print();
		s.clear();
		Sleep(10);
	}
#endif
}