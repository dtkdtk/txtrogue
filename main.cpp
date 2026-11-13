#include "gamecore.hpp"
#include "res_init_nodes.hh"
#include "res_init_tiles.hh"
#include "res_game_map.hh"
#include <iostream>

int main() {
	res_init_nodes();
	res_init_tiles();
	auto map = res_game_map();
	
	std::wcout << map->draw_map() << std::endl;
	std::wcout << "Game map end" << std::endl;
	return 0;
}
