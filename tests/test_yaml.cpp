#include <yaml-cpp/yaml.h>
#include <iostream>

int main(int argc,char** argv){
	YAML::Node node = YAML::LoadFile("../config/test.yaml");
	std::cout << node["name"].as<std::string>() << std::endl;
    std::cout << node["sex"].as<std::string>() << std::endl;
    std::cout << node["age"].as<int>() << std::endl;//18
    std::cout << node["system"]["port"].as<std::string>() << std::endl;
    std::cout << node["system"]["value"].as<std::string>() << std::endl;
    for(auto it = node["system"]["int_vec"].begin(); it != node["system"]["int_vec"].end(); ++it){
		std::cout << *it <<" ";
	}
	std::cout << std::endl;
	return 0;
}
