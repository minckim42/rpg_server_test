#include "Screen.hpp"
using namespace std;

/*##############################################################################

	Screen

##############################################################################*/
/*---------------------------------
	Constructor and destructor
---------------------------------*/	
Screen::Screen(int width, int height)
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
	cleared = new char[(width + 1) * height + 1];
	memcpy(cleared, pixel, sizeof(char) * ((width + 1) * height + 1));
}
//------------------------------------------------------------------------------
Screen::~Screen()
{
	if (pixel)
	{
		delete[] pixel;
		pixel = nullptr;
	}
	if (cleared)
	{
		delete[] cleared;
		cleared = nullptr;
	}
};

/*---------------------------------
	Methods
---------------------------------*/	
void		Screen::set_cursor_position(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), \
		{
			static_cast<short int>(x), 
			static_cast<short int>(y)
		}
	);
}
//------------------------------------------------------------------------------
void		Screen::clear_buffer()
{
	memcpy(pixel, cleared, sizeof(char) * ((width + 1) * height + 1));
}
//------------------------------------------------------------------------------
void		Screen::clear_screen()
{
	system("cls");
}
//------------------------------------------------------------------------------
char&		Screen::get_pixel(int x, int y)
{
	return pixel[(width + 1) * (height - 2 - y) + x];
}
//------------------------------------------------------------------------------
void		Screen::draw_string(int x, int y, const string& str)
{
	if (x < 0 || width <= x || y < 0 || height <= y)
	{
		// throw Exception
		return ;
	}
	// set_cursor_position(x, y);
	int		len = min(width - x, str.length());
	memcpy(&(get_pixel(x, y)), str.data(), len);
}
//------------------------------------------------------------------------------
void		Screen::draw_point(int x, int y, char c)
{
	if (x < 0 || width <= x || y < 0 || height <= y)
		return ;
	get_pixel(x, y) = c;
}
//------------------------------------------------------------------------------
void		Screen::print()
{
	set_cursor_position(0, 0);
	// SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
	cout.write(pixel, (width + 1) * height);
}