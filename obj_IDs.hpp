#pragma once

namespace TileID {
	enum Enum : int {
		NONE,
		WALL,
		DOOR,
		WATER,
		STONEFLOOR
	};
}

namespace NodeCategory {
	enum Enum : unsigned char {
		NONE,
		PRIMARY_ACTION
	};
}

namespace NodeID {
	enum Enum : int {
		NONE,
		ACTION,
		ANIMATION
	};
}

namespace AttrID {
	enum AttrID : int {
		NONE
	};
}

namespace PropID {
	enum PropID : int {
		NONE
	};
}

union NodeKind {
	unsigned char category;
	int id;
};
inline int trimNodeCat(int id) {
	NodeKind k{ id };
	k.category = 0;
	return k.id;
}
