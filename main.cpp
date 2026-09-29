#include "gamecore.hpp"
#include "res_init_nodes.hh"
#include "res_init_tiles.hh"
#include <iostream>

int main() {
	res_init_nodes();
	res_init_tiles();
	auto & tile_templ = getTileTemplates();
	GameMap map(40, 20);
	map.tile_at(0,0) = Tile(tile_templ[1]);
	for (int Y = 5; Y <= 15; Y++)
		for (int X = 10; X <= 30; X++)
			map.tile_at(X,Y) = Tile(tile_templ[1]);
	std::wcout << map.draw_map() << std::endl;
	std::wcout << "Game map end" << std::endl;
	return 0;
}
