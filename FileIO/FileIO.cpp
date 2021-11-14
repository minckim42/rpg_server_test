#include "FileIO.hpp"

using namespace std;

string	blank_remover(string& str)
{
	string			result;
	stringstream	ss(str);
	ss >> result;
	return result;
}
//------------------------------------------------------------------------------
FileData	load_file(const string& path)
{
	ifstream		ifs;
	string			line;
	FileData			result;

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
void	save_file(const string& path, const FileData& data)
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
string	get_data_from_file(const FileData& data, const string& key, int idx)
{
	auto	it = data.find(key);
	if (it == data.end())
	{
		throw string("FileData: key not found: ") + key;
	}
	if (it->second.size() < idx)
	{
		throw string("FileData: Out of index: ") + key + " : " + to_string(idx);
	}
	return it->second[idx];
}