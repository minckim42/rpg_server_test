#include "../Movable/Movable.hpp"
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

	/*---------------------------------
		Constructor & Destructor
	---------------------------------*/
	public:
	PlayerBase()=default;
	PlayerBase(const Movable& movable, uint32_t id, std::string name);
	

	void		Move(float time);
};

class BulletBase
{
	public:
	Movable		movable;
	uint32_t	id;
};