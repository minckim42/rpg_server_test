#pragma once
#include <chrono>
#include <Windows.h>
#include <iostream>
class FrameManager
{
	typedef std::chrono::system_clock	clock;
	typedef std::chrono::milliseconds	milliseconds;
	
	clock::time_point	t0;
	double				time;

	/*---------------------------------
		Constructor and destructor
	---------------------------------*/	
	public:
	FrameManager();
	FrameManager(double time);
	
	/*---------------------------------
		Methods
	---------------------------------*/	
	void	sleep();

	template <typename F, typename... Args>
	bool	set_interval(F& todo, Args& ... args)
	{
		if (std::chrono::duration_cast<milliseconds>(clock::now() - t0).count() < time)
			return false;
		t0 = clock::now();
		todo((args)...);
		return true;
	}

	template <typename F, typename... Args>
	bool	set_interval(F& todo, Args&& ... args)
	{
		if (std::chrono::duration_cast<milliseconds>(clock::now() - t0).count() < time)
			return false;
		t0 = clock::now();
		todo((args)...);
		return true;
	}
};