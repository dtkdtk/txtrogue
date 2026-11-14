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

inline std::map<unsigned int, NodeSource> & getKnownNodeSources();
inline std::vector<Tile> & getTileTemplates();


/*
	Described node IDs are used only inside the Tile
	 to bind the Node to some action category.
	To get NodeSource from NodeRef, you should use partial (clean) ID.
*/

static constexpr unsigned int _NodeCategoryShift =
	(sizeof(int) - sizeof(unsigned char)) * 8;
static constexpr unsigned int _NodeCategoryMask = 0xff << _NodeCategoryShift;

class NodeDescribedId {

public:
	unsigned int full_id;

	NodeDescribedId(unsigned int full_id_) : full_id{ full_id_ } {}
	NodeDescribedId(unsigned char category_, unsigned int partial_id_) {
		if ((partial_id_ & _NodeCategoryMask) != 0)
			throw std::runtime_error(
				"Got invalid partial_id_ in NodeKind; did you confuse it with full_id ?"
			);
		full_id = partial_id_;
		set_category(category_);
	}

	unsigned char category() const noexcept {
		return static_cast<unsigned char>(
			(full_id & _NodeCategoryMask) >> _NodeCategoryShift
			);
	}
	void set_category(unsigned char cat) noexcept {
		unsigned int cat_w = static_cast<unsigned int>(cat);
		cat_w = cat_w << _NodeCategoryShift;
		full_id |= cat_w;
	}
	unsigned int id() const noexcept {
		return full_id & ~_NodeCategoryMask;
	}
};


struct NodeSource {
	const NodeExecFn executor;

	NodeSource(NodeExecFn e)
		: executor(e) {}
};


class NodeRef {

private:
	NodeDescribedId _d_id;

public:
	NodeRef(NodeDescribedId d_id_)
		: _d_id(d_id_) {}
	NodeRef()
		: _d_id(0) {}

	NodeDescribedId get_id() const noexcept {
		return _d_id;
	}
	NodeSource & get_source() const {
		unsigned int id = _d_id.id();
		auto & kns = getKnownNodeSources();
		if (id < 0 || id >= kns.size()) {
			throw std::runtime_error(std::format("Invalid node id: {}", id));
		}
		return kns.at(id);
	}
	void execute(Tile & tile) const {
		NodeSource & s = get_source();
		s.executor(tile);
	}
	bool is_none() const noexcept {
		return _d_id.id() == NodeID::NONE;
	}
};



class Tile {

public:
	using KvProperty = MapSpan<int, PropertyValue>::Entry;

	Cell cell{};
	short map_layer; //needed to see other layer tiles
	DynSpan<NodeRef> nodes{};
	DynSpan<int> attrs{};
	MapSpan<int, PropertyValue> props{};

	Tile() = default;
	Tile(
		Cell cell_,
		short map_layer_,
		std::initializer_list<NodeRef> nodes_,
		std::initializer_list<int> attrs_,
		std::initializer_list<KvProperty> props_
	) : cell(cell_), map_layer(map_layer_),
		  nodes(nodes_), attrs(attrs_), props(props_) {}

	/* returns `false` if reached non-void empty or the map border */
	bool try_move(RelCoord dest) {
		//TODO
		return true;
	}

	/* returns `false` only if reached the map border */
	bool force_move(RelCoord dest) {
		//TODO
		return true;
	}

	std::vector<NodeRef> get_category_nodes(unsigned char cat) const {
		//1. calculate nodes count (to get vector size)
		//2. allocate vector & fill it with nodes
		size_t count = 0;
		for (const auto & n : nodes) {
			NodeDescribedId k = n.get_id();
			if (k.category() == cat) count++;
		}
		if (count == 0) return {};

		std::vector<NodeRef> vct{};
		vct.reserve(count);
		for (const auto & n : nodes) {
			NodeDescribedId k = n.get_id();
			if (k.category() == cat) vct.push_back(n);
		}
		return vct;
	}

	/* may return NONE node if not found */
	NodeRef get_id_node(unsigned int id) const {
		if (NodeDescribedId(id).id() == NodeID::NONE)
			throw std::runtime_error("Cannot get node with NONE id");
		for (const auto & n : nodes) {
			if (n.get_id().id() == id) return n;
		}
		return NodeRef(NodeDescribedId(NodeID::NONE));
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




inline std::map<unsigned int, NodeSource> & getKnownNodeSources() {
	static std::map<unsigned int, NodeSource> _v{};
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
	int get_health() const {
		//TODO: get from prop
		return 1;
	}
	bool is_alive() const {
		//TODO
		return true;
	}
	bool self_damage_wall(RelCoord XY) { //layer 0
		//TODO, + check range & check ability
		return true;
	}
	bool self_damage_object(RelCoord XY) {
		return self_damage_map_layer(XY, MapLayer::OBJECTS);
	}
	bool self_damage_map_layer(RelCoord XY, int layer) {
		//TODO, + check range & check ability
		return true;
	}
};



class PlayerState : public EntityState {

public:

};



void requireTileProperty(Tile & t, int id) {
	if (!t.props.contains(id))
		throw std::runtime_error(std::format(
			"Missing prop {} on tile",
			id
		));
}
void requireTileNode(Tile & t, unsigned int id) {
	if (t.get_id_node(id).is_none())
		throw std::runtime_error(std::format(
			"Missing node {} on tile",
			id
		));
}
