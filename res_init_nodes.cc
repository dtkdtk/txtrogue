#include "res_init_nodes.hh"

void noop(Tile & tile) {}

void res_init_nodes() {
	auto & m = getKnownNodeSources();
	m.insert({
		NodeID::NONE,
		NodeSource(noop)
	});
	m.insert({
		NodeID::DOOR_ACTION,
		NodeSource([](Tile & tile) {
			if (tile.attrs.has_value(AttrID::DOOR_OPEN)) {
				//then, close
				tile.cell.ch = DoorStateIcons[0];
				tile.attrs.push_back(AttrID::SOLID);
				tile.attrs.erase(AttrID::DOOR_OPEN);
			}
			else {
				//then, open
				tile.cell.ch = DoorStateIcons[1];
				tile.attrs.erase(AttrID::SOLID);
				tile.attrs.push_back(AttrID::DOOR_OPEN);
			}
		})
	});
}
