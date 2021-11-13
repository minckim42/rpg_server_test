#pragma once
#include <windows.h>
#include <chrono>
#include <map>
#include <vector>

/*##############################################################################

	Callback

##############################################################################*/
class Callback
{
public:
	Callback() = default;
	virtual void	behavior(int key);
	virtual void	behavior(int key, int other_key);
};


/*##############################################################################

	KeyManager

##############################################################################*/
class KeyManager
{
	/*==================================
		pulic member types
	==================================*/
	public:
	typedef std::chrono::high_resolution_clock::time_point	time_point;

	/*==================================
		private members
	==================================*/
	private:
	time_point			_time_released;
	bool				_is_released;
	long				_interval;

	// static members
	static time_point	_start;
	/*==================================
		public members
	==================================*/
	public:
	int					key;

	// static members
	static const long	interval_default;

	/*==================================
		canonical
	==================================*/
	public:
	KeyManager(int key, long interval = interval_default);

	/*==================================
		public methods
	==================================*/
	public:
	// 단일 키 입력 확인
	bool	is_pressed();
	bool	is_pressed(Callback& behavior);
	bool	is_pressed(void (*behavior)(int key));
	bool	is_pressed(void (*behavior)(int key, void* arg), void* arg);
	
	// 두 번 연속 입력 확인
	bool	is_pressed_double();
	bool	is_pressed_double(Callback& behavior);
	bool	is_pressed_double(void (*behavior)(int key));
	bool	is_pressed_double(void (*behavior)(int key, void* arg), void* arg);
	
	// 조합 키 입력 확인
	bool	is_pressed_comb(int other_key);
	bool	is_pressed_comb(Callback& behavior, int other_key);
	bool	is_pressed_comb(void (*behavior)(int key, int other_key), int other_key);
	bool	is_pressed_comb(void (*behavior)(int key, int other_key, void* arg), void* arg, int other_key);
};

/*==================================
	non member function
==================================*/
long	time_interval_ms(KeyManager::time_point t0, KeyManager::time_point t1); // 특정 시점으로부터의 시간
long	time_interval_ms(KeyManager::time_point t0); // 프로세스 시작으로부터의 시간