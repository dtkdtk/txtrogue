#include "gamecore.hpp"
#include "res_init_nodes.hh"
#include "res_init_tiles.hh"
#include <iostream>

int main() {
	res_init_nodes();
	res_init_tiles();
	GameMap map(40, 20);
	std::wcout << map.draw_map() << std::endl;
	return 0;
}
