#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

typedef std::map<std::string, std::vector<std::string>>	SaveData;

SaveData	load_file(const std::string& path);
void		save_file(const std::string& path, const SaveData& data);
