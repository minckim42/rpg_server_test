#include "parser.hpp"
#include "../Screen/Screen.hpp"
#include "../KeyManager/KeyManager.hpp"
#include "../FrameManager/FrameManager.hpp"
#include "../Server/PlayerData.hpp"

using namespace std;

void		print_screen(Screen& screen)
{
	screen.print();
}

int			main()
{
	Screen			screen(100, 28);
	FrameManager	fm(100);
	FrameManager	fm_key(50);

	KeyManager	key_up('W');
	KeyManager	key_down('S');
	KeyManager	key_left('A');
	KeyManager	key_right('D');
	KeyManager	key_quit('Q');
	KeyManager	key_start('1');
	KeyManager	key_load('2');
	KeyManager	key_save('3');

	PlayerData	player;

	try
	{
		while (1)
		{
			screen.clear_buffer();
			screen.draw_string(3, 25, "1. start");
			screen.draw_string(3, 24, "2. load");
			screen.draw_string(3, 23, "3. save");
			screen.draw_string(3, 0, "name: ");
			screen.draw_string(9, 0, player.name);

			if (key_start.is_pressed())
				break;
			if (key_load.is_pressed())
			{
				player.load();
				break;
			}

			screen.draw_point(player.position[0], player.position[1], player.shape);
			fm.set_interval(print_screen, screen);
		}

		while (1)
		{
			screen.clear_buffer();
			screen.draw_string(40, 0, "2. load");
			screen.draw_string(80, 0, "3. save");
			screen.draw_string(3, 0, "name: ");
			screen.draw_string(9, 0, player.name);

			if (key_up.is_pressed())
				player.position[1] += player.speed;
			if (key_down.is_pressed())
				player.position[1] -= player.speed;
			if (key_left.is_pressed())
				player.position[0] -= player.speed;
			if (key_right.is_pressed())
				player.position[0] += player.speed;
			if (key_quit.is_pressed())
				exit(0);
			if (key_load.is_pressed())
			{
				player.load();
			}
			if (key_save.is_pressed())
			{
				player.save();
			}
			fm_key.sleep();
			screen.draw_string(0, 26, to_string(player.position[0]));
			screen.draw_string(10, 26, to_string(player.position[1]));
			screen.draw_point(player.position[0], player.position[1], player.shape);
			fm.set_interval(print_screen, screen);
		}
	}
	catch(const std::string e)
	{
		std::cerr << e << '\n';
	}
	



}