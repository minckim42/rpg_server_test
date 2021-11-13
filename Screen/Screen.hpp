#pragma once
#include <iostream>
#include <windows.h>
/*##############################################################################

	Screen

##############################################################################*/
class Screen
{
	/*---------------------------------
		Members
	---------------------------------*/	
	public:
	static int	count;
	int		width;
	int		height;
	char*	pixel;
	char*	cleared;

	/*---------------------------------
		Constructor and destructor
	---------------------------------*/	
	private:
	Screen();
	Screen(Screen& x);
	Screen(const Screen& x);
	Screen(Screen&& x);
	Screen&		operator=(const Screen& x);
	Screen&		operator=(Screen&& x);
	public:
	Screen(int width, int height);
	~Screen();

	/*---------------------------------
		Methods
	---------------------------------*/	
	public:
	void		set_cursor_position(int x, int y);
	void		clear_buffer();
	void		clear_screen();
	char&		get_pixel(int x, int y);
	void		draw_string(int x, int y, const std::string& str);
	void		draw_point(int x, int y, char c);
	void		draw_line();
	void		print();
};