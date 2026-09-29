#include "res_init_tiles.hh"

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		Tile(Cell(L' '), {}, {}, {{}}), //Void
		Tile(Cell(L'8'), {}, {}, {{}})
	};
	for (const Tile & t : tiles)
		tt.push_back(t);
}

