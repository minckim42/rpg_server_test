#include "../Movable/Movable.hpp"
#include "../Protocol/Protocol.hpp"
#include <string>

class PlayerBase
{
	/*---------------------------------
		Member
	---------------------------------*/
	public:
	Movable			movable;
	uint32_t		id;
	std::string		name;
	char			shape;

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	public:
	PlayerBase()=default;
	PlayerBase(const Movable& movable, uint32_t id, std::string name, char shape);

	ResGamePlayer	cast_response();
};

class BulletBase
{
	public:
	Movable		movable;
	uint32_t	id;
};