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
