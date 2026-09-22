#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include "u_dynspan.hpp"
#include "u_mapspan.hpp"
#include "u_grid.hpp"
#include "u_basetypes.hpp"
#include "u_console.hpp"

struct NodeSource;
class NodeRef;
class Tile;
using NodeExecFn = void(*)(Tile &);
using PropertyValue = uint64_t;



struct NodeSource {
	NodeSource(NodeExecFn e)
		: executor{ e } {}
	const NodeExecFn executor;
};

inline std::vector<NodeSource> KnownNodeKinds = {};



class NodeRef {

public:
	NodeRef(int kind)
		: _kind{ kind } {}
	NodeRef() = default;

	int get_kind() const noexcept {
		return _kind;
	}
	NodeSource & get_source() const {
		if (_kind <= 0 || _kind > KnownNodeKinds.size()) {
			throw std::runtime_error(std::format("Invalid node kind: {}", _kind));
		}
		return KnownNodeKinds[_kind - 1];
	}
	void execute(Tile & tile) const {
		NodeSource & s = get_source();
		s.executor(tile);
	}

private:
	int _kind = 0;
};



class Tile {

public:
	Cell cell{};
	DynSpan<NodeRef> nodes{};
	DynSpan<Key8> attrs{};
	MapSpan<Key8, PropertyValue> properties{};
};



class GameMap {
	using uint = unsigned int;
	using Grid = ::Grid<Tile, uint>;

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

	void draw_into(
		Console & console,
		size_t origin_x = 0,
		size_t origin_y = 0
	) const {
		const size_t cw = console.width();
		const size_t ch = console.height();
		const size_t mw = _data.width();
		const size_t mh = _data.height();

		for (size_t y = 0; y < mh; ++y) {
			const size_t ty = origin_y + y;
			if (ty >= ch) break;
			for (size_t x = 0; x < mw; ++x) {
				const size_t tx = origin_x + x;
				if (tx >= cw) break;
				console.at(tx, ty) = _data.at(static_cast<uint>(x),
					static_cast<uint>(y)).cell;
			}
		}
	}

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

private:
	Grid _data;
};
