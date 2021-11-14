#include "parser.hpp"

using namespace std;

string	blank_remover(string& str)
{
	string			result;
	stringstream	ss(str);
	ss >> result;
	return result;
}
//------------------------------------------------------------------------------
SaveData	load_file(const string& path)
{
	ifstream		ifs;
	string			line;
	SaveData			result;

	ifs.open(path);
	if (ifs.is_open() == false)
	{
		throw string("Failed to open file: ") + path;
	}
	
	while (ifs.good())
	{
		getline(ifs, line);

		stringstream	ss(line);
		string			key;
		string			value;

		getline(ss, key, ':');
		key = blank_remover(key);
		result[key] = vector<string>();
		while (ss.good())
		{
			getline(ss, value, ',');
			value = blank_remover(value);
			result[key].emplace_back(value);
		}
	}
	return result;
}
//------------------------------------------------------------------------------
void	save_file(const string& path, const SaveData& data)
{
	ofstream		ofs;

	ofs.open(path);
	if (ofs.is_open() == false)
	{
		throw string("Failed to save file: ") + path;
	}
	
	for (pair<const string, vector<string>> line : data)
	{
		ofs << line.first << ": ";
		for (vector<string>::iterator it = line.second.begin() ; it != line.second.end() ; ++it)
		{
			ofs << *it;
			if (it + 1 != line.second.end())
				ofs << ", ";
			else
				ofs << endl;
		}
	}
}
//------------------------------------------------------------------------------