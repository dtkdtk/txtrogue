#include "res_init_tiles.hh"

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		Tile(Cell(L' '), WALLS, {}, {}, {{}}), //NONE
		Tile(Cell(L'8'), WALLS, {}, {}, {{}}), //WALL
		Tile(Cell(L'#'), WALLS, {}, {}, {{}}), //DOOR
		Tile(Cell(L'_'), FLOOR, {}, {}, {{}}), //WATER
		Tile(Cell(L' '), FLOOR, {}, {}, {{}}) //STONEFLOOR
	};
	tt.reserve(sizeof(tiles) / sizeof(Tile));
	for (const Tile & t : tiles)
		tt.push_back(t);
}

