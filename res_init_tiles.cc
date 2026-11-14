#include "res_init_tiles.hh"

#define NODE(cat, id) NodeRef(NodeDescribedId(NodeCategory:: cat, NodeID:: id))

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		// cell map_layer nodes attrs props
		Tile(Cell(L' '), WALLS, {}, {}, {}), //NONE
		Tile(Cell(L'8'), WALLS, {}, {}, {}), //WALL
		Tile(Cell(L'#'), WALLS, {
			NODE(PRIMARY_ACTION, DOOR_ACTION)
		}, {}, {
			{}
		}), //DOOR
		Tile(Cell(L'_'), FLOOR, {}, {}, {}), //WATER
		Tile(Cell(L' '), FLOOR, {}, {}, {}) //STONEFLOOR
	};
	tt.reserve(sizeof(tiles) / sizeof(Tile));
	for (const Tile & t : tiles)
		tt.push_back(t);
}

