#include "PlayerData.hpp"

using namespace std;
/*##############################################################################

	PlayerData

##############################################################################*/
PlayerData::PlayerData()
:	position{1, 2},
	shape('C'),
	name("nonamed"),
	speed(0.3)
{}
//------------------------------------------------------------------------------
void	PlayerData::load()
{
	FileData	data = load_file("save.txt");
	set_shape(data);
	set_position(data);
	set_name(data);
	set_velocity(data);
}
//------------------------------------------------------------------------------
void	PlayerData::save()
{
	FileData		data;
	data["position"] = vector<string>();
	data["position"].push_back(to_string(position[0]));
	data["position"].push_back(to_string(position[1]));
	
	data["name"] = vector<string>();
	data["name"].push_back(name);

	data["shape"] = vector<string>();
	data["shape"].push_back(string("") + shape);

	data["speed"] = vector<string>();
	data["speed"].push_back(to_string(speed));

	save_file("save.txt", data);
}
//------------------------------------------------------------------------------
void	PlayerData::set_shape(FileData& data)
{
	if (data.find("shape") == data.end() 
			|| !data["shape"].size() 
			|| !data["shape"][0].length()
	)
	{
		shape = 'A';
	}
	else
	{
		shape = data["shape"][0][0];
	}
}
//------------------------------------------------------------------------------
void	PlayerData::set_position(FileData& data)
{
	if (data.find("position") == data.end())
		throw string("Failed to load data: position empty");
	if (data["position"].size() < 2)
		throw string("Failed to load data") + to_string(data["position"].size());
	for (int i = 0 ; i < 2 ; i++)
	{
		position[i] = stof(data["position"][i]);
		if (position[i] < -100 || 100 < position[i])
			throw string("Failed to load data: position range");
	}
}
//------------------------------------------------------------------------------
void	PlayerData::set_name(FileData& data)
{
	if (data.find("name") == data.end())
		throw string("Failed to load data: name key is empty");
	if (data["name"].size() == 0 
		|| data["name"][0].length() > 19 
		|| data["name"][0].length() == 0
	)
		throw string("Failed to load data: name is empty or too long");
	string	name_dup = data["name"][0];
	memcpy(name, name_dup.data(), name_dup.length());
	name[name_dup.length()] = 0;
}
//------------------------------------------------------------------------------
void	PlayerData::set_velocity(FileData& data)
{
	if (data.find("speed") == data.end())
		throw string("Failed to load data: speed is empty");
	if (data["speed"].size() == 0)
		throw string("Failed to load data: speed is empty");
	speed = stof(data["speed"][0]);
	if (speed > 10)
		throw string("Failed to load data: speed is too high");
}
//------------------------------------------------------------------------------



uint32_t	Message::get_hash()
{
	uint32_t	tmp = 0;
	uint32_t	len_data = size();
	for (uint32_t i = 0 ; i < len_data ; i++)
	{
		tmp *= body[i] * i + seed * i;
	}
	return tmp;	
}
//------------------------------------------------------------------------------
void	Message::set_hash()
{
	seed = 293847; // need random
	hash = get_hash();
}
//------------------------------------------------------------------------------
bool	Message::check_hash()
{
	return get_hash() == hash;
}
//------------------------------------------------------------------------------
int		Message::body_size()
{
	switch (type)
	{
		case CONNECT_REQUEST:
			return sizeof(ConnectRequest);
		case CONNECT_RESPONSE:
			return sizeof(ConnectResponse);
		case INSTRUCTION:
			return sizeof(Instruction);
		case PLAYER_DATA:
			return sizeof(PlayerData);
	}
	return 0;
}
//------------------------------------------------------------------------------
int		Message::size()
{
	return offsetof(Message, body) + body_size();
}