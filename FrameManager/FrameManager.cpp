#include "FrameManager.hpp"

FrameManager::FrameManager()
:	time(100)
{}

FrameManager::FrameManager(double time)
:	time(time)
{
	t0 = clock::now();
}

void	FrameManager::sleep()
{
	double	remain;
	while ((remain = time - std::chrono::duration_cast<milliseconds>(clock::now() - t0).count()) > 0)
	{
		Sleep(remain / 8);
	}
	t0 = clock::now();
}