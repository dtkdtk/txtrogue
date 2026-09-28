#include "res_init_tiles.hh"

void res_init_tiles() {
	auto & tt = getTileTemplates();
	tt.push_back(
		Tile(Cell(L' '), {}, {}, {}), //Void
		Tile(Cell(L'8'), {}, {}, {})
	);
}

