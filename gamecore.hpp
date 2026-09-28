#pragma once
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include "u_dynspan.hpp"
#include "u_mapspan.hpp"
#include "u_grid.hpp"
#include "u_basetypes.hpp"

struct NodeSource;
class NodeRef;
class Tile;
using NodeExecFn = void(*)(Tile &);
using PropertyValue = uint64_t;
using KvProperty = std::pair<int, PropertyValue>;
std::vector<NodeSource> & getKnownNodeKinds();
std::vector<Tile> & getTileTemplates();


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
	Cell cell{};
	DynSpan<NodeRef> nodes{};
	DynSpan<int> attrs{};
	MapSpan<int, PropertyValue> props{};

	Tile() = default;
	Tile(Cell cell_,
		std::initializer_list<NodeRef> nodes_,
		std::initializer_list<int> attrs_,
		std::initializer_list<KvProperty> props_
	) : cell(cell_), nodes(nodes_), attrs(attrs_), props(props_) {}
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




std::vector<NodeSource> & getKnownNodeKinds() {
	static std::vector<NodeSource> _v{};
	return _v;
}
std::vector<Tile> & getTileTemplates() {
	static std::vector<Tile> _v{};
	return _v;
}
