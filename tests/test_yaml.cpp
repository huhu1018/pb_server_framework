#include <iostream>
#include "config.h"

int main(int argc,char** argv){


	YAML::Node root = YAML::LoadFile("../config/test.yaml");
	std::list<std::pair<std::string, const YAML::Node>> all_nodes;
	// 将root中的结点进行解析，存放到all_nodes中
	ListAllMember("", root, all_nodes);
	// 遍历输出all_nodes
	for (auto it = all_nodes.begin(); it != all_nodes.end(); it++){
		std::cout << it->first << std::endl;
	}
	return 0;
}
