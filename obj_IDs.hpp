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
	enum Enum : unsigned int {
		NONE,
		DOOR_ACTION
	};
}

namespace AttrID {
	enum AttrID : int {
		NONE,
		SOLID,
		DOOR_OPEN,
	};
}

namespace PropID {
	enum PropID : int {
		NONE
	};
}
