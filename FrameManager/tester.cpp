#include "FrameManager.hpp"
#include <iostream>
#include <thread>

using namespace std;

void		foo(int& i)
{
	cout << i++ << endl;
}

void		bar()
{
	cout << "bar" << endl;
}

int			main()
{
	FrameManager	fm(100);

	int				i = 0;

	while (1)
	{
		 fm.set_interval(foo, i);
		 fm.set_interval(bar);
	}
}