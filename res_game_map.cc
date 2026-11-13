#include "res_game_map.hh"

std::unique_ptr<GameMap> res_game_map() {
	std::unique_ptr<GameMap> m = std::make_unique<GameMap>(40, 20);
	auto & tile_templ = getTileTemplates();

	m->tile_at(0, 0) = Tile(tile_templ[1]);
	for (int Y = 5; Y <= 15; Y++)
		for (int X = 10; X <= 30; X++)
			m->tile_at(X, Y) = Tile(tile_templ[1]);

	return m;
}
