#include "res_init_tiles.hh"

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		Tile(Cell(L' '), {}, {}, {{}}), //NONE
		Tile(Cell(L'8'), {}, {}, {{}}), //WALL
		Tile(Cell(L'#'), {}, {}, {{}})
	};
	tt.reserve(sizeof(tiles) / sizeof(Tile));
	for (const Tile & t : tiles)
		tt.push_back(t);
}

