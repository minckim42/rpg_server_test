#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

typedef std::map<std::string, std::vector<std::string>>	FileData;

FileData	load_file(const std::string& path);
void		save_file(const std::string& path, const FileData& data);
std::string	get_data_from_file(const FileData& data, const std::string& key, int idx = 0);