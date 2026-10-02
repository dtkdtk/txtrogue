#pragma once
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <optional>
#include <memory>
#include <map>
#include "u_dynspan.hpp"
#include "u_mapspan.hpp"
#include "u_grid.hpp"
#include "u_basetypes.hpp"
#include "obj_IDs.hpp"

struct NodeSource;
class NodeRef;
class Tile;
class GameMap;

using NodeExecFn = void(*)(Tile &);
using PropertyValue = uint64_t;

/*
	Relative Coordinates
```
	      -3
	      -2
	      -
  -3 -2 - A + +2 +3
		  +
		  +2
		  +3
```
*/
using RelCoord = Coord<short>;

inline std::vector<NodeSource> & getKnownNodeKinds();
inline std::vector<Tile> & getTileTemplates();


struct NodeSource {
	const NodeExecFn executor;

	NodeSource(NodeExecFn e)
		: executor{ e } {}
};



class NodeRef {

private:
	int _kind = 0;

public:
	NodeRef(int kind)
		: _kind{ kind } {}
	NodeRef() = default;

	int get_kind() const noexcept {
		return _kind;
	}
	NodeSource & get_source() const {
		auto & knk = getKnownNodeKinds();
		if (_kind <= 0 || _kind > knk.size()) {
			throw std::runtime_error(std::format("Invalid node kind: {}", _kind));
		}
		return knk[_kind - 1];
	}
	void execute(Tile & tile) const {
		NodeSource & s = get_source();
		s.executor(tile);
	}
};



class Tile {

public:
	using KvProperty = MapSpan<int, PropertyValue>::Entry;

	Cell cell{};
	short mapLayer; //needed to see other layer tiles
	DynSpan<NodeRef> nodes{};
	DynSpan<int> attrs{};
	MapSpan<int, PropertyValue> props{};

	Tile() = default;
	Tile(Cell cell_,
		short mapLayer_,
		std::initializer_list<NodeRef> nodes_,
		std::initializer_list<int> attrs_,
		std::initializer_list<KvProperty> props_
	) : cell(cell_), mapLayer(mapLayer_), nodes(nodes_), attrs(attrs_), props(props_) {}

	/* returns `false` if reached non-void empty or the map border */
	bool tryMove(RelCoord dest) {
		//TODO
		return true;
	}

	/* returns `false` only if reached the map border */
	bool forceMove(RelCoord dest) {
		//TODO
		return true;
	}

	std::span<NodeRef> getCategoryNodes(NodeCategory cat) const {
		//1. calculate nodes count (to get span size)
		//2. allocate span & fill it with nodes
	}
};



enum MapLayer : short {
	FLOOR = -1,
	WALLS = 0,
	OBJECTS = 1
};



class GameMap {

private:
	using uint = unsigned int;
	using Grid = ::Grid<Tile, uint>;
	Grid _data;

public:
	GameMap(uint size_X, uint size_Y)
		: _data(size_X, size_Y, Tile()) {}
	auto tiles_flat()						noexcept { return _data.flat(); }
	auto tiles_flat()				const	noexcept { return _data.flat(); }
	auto tile_rows()						noexcept { return _data.rows(); }
	auto tile_rows()				const	noexcept { return _data.rows(); }
	auto tile_columns()						noexcept { return _data.columns(); }
	auto tile_columns()				const	noexcept { return _data.columns(); }
	auto & tile_at(uint X, uint Y)					 { return _data.at(X, Y); }
	auto & tile_at(Grid::coord XY)					 { return _data.at(XY); }
	auto tile_row_at(uint Y)				noexcept { return _data.row_at(Y); }
	auto tile_row_at(uint Y)		const	noexcept { return _data.row_at(Y); }
	auto tile_column_at(uint X)				noexcept { return _data.column_at(X); }
	auto tile_column_at(uint X)		const	noexcept { return _data.column_at(X); }

	uint width()  const noexcept { return _data.width(); }
	uint height() const noexcept { return _data.height(); }

	void on_turn() {
		for (auto & tile : tiles_flat()) {
			for (const auto & node : tile.nodes) {
				node.execute(tile);
			}
		}
	}

	//TODO: draw into buffer

	std::wstring draw_map() const {
		std::wstring result{};
		result.reserve(_data.size() + _data.height());
		for (const auto & row_iter : tile_rows()) {
			for (const auto & tile : row_iter) {
				result += tile.cell.ch;
			}
			result += L'\n';
		}
		return result;
	}
};




inline std::vector<NodeSource> & getKnownNodeKinds() {
	static std::vector<NodeSource> _v{};
	return _v;
}
inline std::vector<Tile> & getTileTemplates() {
	static std::vector<Tile> _v{};
	return _v;
}

static inline std::map<short, std::unique_ptr<GameMap>> _GameMapLayers{};
//nullable
inline GameMap * getGameMap(short layer) {
	if (!_GameMapLayers.contains(layer))
		return nullptr;
	return _GameMapLayers[layer].get();

}
inline void setMapLayer(short layer, std::unique_ptr<GameMap> map) {
	_GameMapLayers[layer] = std::move(map);
}



class EntityState : public Tile {

public:
	int getHealth() const {
		//TODO, get from prop
	}
	bool isAlive() const {
		//TODO
		return true;
	}
	bool selfDamageWall(RelCoord XY) { //layer 0
		//TODO, + check range & check ability
		return true;
	}
	bool selfDamageObject(RelCoord XY) {
		return selfDamageMapLayer(XY, MapLayer::OBJECTS);
	}
	bool selfDamageMapLayer(RelCoord XY, int layer) {
		//TODO, + check range & check ability
		return true;
	}
};



class PlayerState : public EntityState {

public:

};
