#include "res_init_tiles.hh"

#define NODE(cat, id) NodeRef(NodeDescribedId(NodeCategory:: cat, NodeID:: id))

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		// cell map_layer nodes attrs props
		Tile(Cell(L' '), {}, {}, {}), //NONE
		Tile(Cell(L'8'), {}, { AttrID::SOLID }, {}), //WALL
		Tile(Cell(DoorStateIcons[0]), {
			NODE(PRIMARY_ACTION, DOOR_ACTION)
		}, { AttrID::SOLID }, {{}}), //DOOR
		Tile(Cell(L'_'), {}, {}, {}), //WATER
		Tile(Cell(L' '), {}, {}, {}) //STONEFLOOR
	};
	tt.reserve(sizeof(tiles) / sizeof(Tile));
	for (const Tile & t : tiles)
		tt.push_back(t);
}

