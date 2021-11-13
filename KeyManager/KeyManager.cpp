#include "KeyManager.hpp"

using namespace std;
using namespace chrono;

/*##############################################################################

	Callback

##############################################################################*/
void	Callback::behavior(int key) {}
void	Callback::behavior(int key, int other_key) {}

/*##############################################################################

	KeyManager

##############################################################################*/
/*==================================
	initialize static members
==================================*/
const long	KeyManager::interval_default = 300;
KeyManager::time_point	KeyManager::_start(high_resolution_clock::now());

/*==================================
	canonical
==================================*/

KeyManager::KeyManager(int key, long interval) :
	key(key),
	_is_released(true),
	_interval(interval),
	_time_released(high_resolution_clock::now() - std::chrono::duration<long>(interval))
{}

/*==================================
	public methods
==================================*/

bool	KeyManager::is_pressed()
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed(Callback& behavior)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		behavior.behavior(key);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed(void (*behavior)(int key))
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		behavior(key);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed(void (*behavior)(int key, void* arg), void* arg)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		behavior(key, arg);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_double()
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (time_interval_ms(_time_released) <= _interval && _is_released == true)
		{
			_is_released = false;
			return true;
		}
		_is_released = false;
	}
	else
	{
		if (_is_released == false)
		{
			_time_released = high_resolution_clock::now();
			_is_released = true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_double(Callback& behavior)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (time_interval_ms(_time_released) <= _interval && _is_released == true)
		{
			behavior.behavior(key);
			_is_released = false;
			return true;
		}
		_is_released = false;
	}
	else
	{
		if (_is_released == false)
		{
			_time_released = high_resolution_clock::now();
			_is_released = true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_double(void (*behavior)(int key))
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (time_interval_ms(_time_released) <= _interval && _is_released == true)
		{
			behavior(key);
			_is_released = false;
			return true;
		}
		_is_released = false;
	}
	else
	{
		if (_is_released == false)
		{
			_time_released = high_resolution_clock::now();
			_is_released = true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_double(void (*behavior)(int key, void* arg), void* arg)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (time_interval_ms(_time_released) <= _interval && _is_released == true)
		{
			behavior(key, arg);
			_is_released = false;
			return true;
		}
		_is_released = false;
	}
	else
	{
		if (_is_released == false)
		{
			_time_released = high_resolution_clock::now();
			_is_released = true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_comb(int other_key)
{
	if (GetAsyncKeyState(key) & 0x8000 && GetAsyncKeyState(other_key) & 0x8000)
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_comb(Callback& behavior, int other_key)
{
	if (GetAsyncKeyState(key) & 0x8000 && GetAsyncKeyState(other_key) & 0x8000)
	{
		behavior.behavior(key, other_key);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_comb(void (*behavior)(int key, int other_key), int other_key)
{
	if (GetAsyncKeyState(key) & 0x8000 && GetAsyncKeyState(other_key) & 0x8000)
	{
		behavior(key, other_key);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool	KeyManager::is_pressed_comb(void (*behavior)(int key, int other_key, void* arg), void* arg, int other_key)
{
	if (GetAsyncKeyState(key) & 0x8000 && GetAsyncKeyState(other_key) & 0x8000)
	{
		behavior(key, other_key, arg);
		return true;
	}
	return false;
}

/*==================================
	non member function
==================================*/

long	time_interval_ms(KeyManager::time_point t0, KeyManager::time_point t1)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		t1 - t0).count();
}
//------------------------------------------------------------------------------
long	time_interval_ms(KeyManager::time_point t0)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		high_resolution_clock::now() - t0).count();
}