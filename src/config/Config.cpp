#include <iostream>
#include <fstream>
#include "config/Config.h"
#include "tools/json.h"

json Config::GetConfig(std::string path) {
	std::ifstream iJsonFile(path);
	json jConfig;
	iJsonFile >> jConfig;
	return jConfig;
}