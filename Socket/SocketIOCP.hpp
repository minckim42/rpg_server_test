#pragma once
#include "__SocketBase.hpp"
#include "SocketEndpoint.hpp"
#include <vector>
#include <thread>
/*##############################################################################

	SocketIOCP

##############################################################################*/

class SocketIOCP: public __SocketBase
{
	/*---------------------------------
		Members
	---------------------------------*/
	public:
	HANDLE						completion_port;
	std::vector<std::thread>	threads;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	private:
	SocketIOCP(const SocketIOCP&);
	SocketIOCP(SocketIOCP&& x);
	SocketIOCP&		operator=(const SocketIOCP&);
	SocketIOCP&		operator=(SocketIOCP&& x);

	public:
	SocketIOCP(int port);

	/*---------------------------------
		Methods
	---------------------------------*/		
	void				create_iocp();
	void				bind();
	void				listen(int backlog);
	SocketEndpoint*		accept();
	void				connect_iocp(SocketEndpoint* endpoint);

	/*---------------------------------
		Templates
	---------------------------------*/	
	template	<typename F, typename... Args>
	void		init_thread(F& todo, Args& ... args)
	{
		SYSTEM_INFO			sys_info;
		GetSystemInfo(&sys_info);

		for (unsigned int i = 0 ; i < sys_info.dwNumberOfProcessors ; i++)
		{
			threads.emplace_back(F, (args)...);
			threads.back().detach();
		}
	}

	template	<typename F, typename... Args>
	void		init_thread(F& todo, Args&& ... args)
	{
		SYSTEM_INFO			sys_info;
		GetSystemInfo(&sys_info);

		for (unsigned int i = 0 ; i < sys_info.dwNumberOfProcessors ; i++)
		{
			threads.emplace_back(F, (args)...);
			threads.back().detach();
		}
	}

};