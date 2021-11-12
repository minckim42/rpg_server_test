#include "Client.hpp"
#include <iostream>
#include <thread>

using namespace std;

int		main(int argc, char** argv)
{
	try
	{
		init_wsadata();

		Client		client("127.0.0.1", 9090);


		thread		thread_controller(ref(client), Client::Controller());
		thread_controller.detach();

		client.run();
	}
	catch(const exception& e)
	{
		cerr << e.what() << '\n';
	}
	catch(const string& e)
	{
		cerr << e << '\n';
	}
	cleanup_wsadata();
}