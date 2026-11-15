#include "res_init_tiles.hh"

#define NODE(cat, id) NodeRef(NodeDescribedId(NodeCategory:: cat, NodeID:: id))

void res_init_tiles() {
	auto & tt = getTileTemplates();
	Tile tiles[] = {
		// cell map_layer nodes attrs props
		Tile(Cell(L' '), WALLS, {}, {}, {}), //NONE
		Tile(Cell(L'8'), WALLS, {}, { AttrID::SOLID }, {}), //WALL
		Tile(Cell(DoorStateIcons[0]), WALLS,{
			NODE(PRIMARY_ACTION, DOOR_ACTION)
		}, { AttrID::SOLID }, {{}}), //DOOR
		Tile(Cell(L'_'), FLOOR, {}, {}, {}), //WATER
		Tile(Cell(L' '), FLOOR, {}, {}, {}) //STONEFLOOR
	};
	tt.reserve(sizeof(tiles) / sizeof(Tile));
	for (const Tile & t : tiles)
		tt.push_back(t);
}

