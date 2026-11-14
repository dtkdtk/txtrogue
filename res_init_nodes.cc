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
			
		})
	});
}
