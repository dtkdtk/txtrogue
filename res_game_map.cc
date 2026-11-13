#include "res_game_map.hh"

std::unique_map<GameMap> res_game_map() {
	GameMap m(40, 40);

	return std::make_unique(m);
}
