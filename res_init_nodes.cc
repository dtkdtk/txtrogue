#include "res_init_nodes.hh"

void noop(Tile & tile) {}

void res_init_nodes() {
	auto & m = getKnownNodeSources();
	unsigned int idc = 0; //unique ID counter
	m.insert({
		idc++,
		NodeSource(noop)
	});
}
