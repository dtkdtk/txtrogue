#pragma once

enum class TileID : int {
	NONE,
	WALL,
	DOOR,
	WATER,
	STONEFLOOR
};

enum class NodeID : int {
	NONE,
	ACTION,
	ANIMATION
};

enum class AttrID : int {
	NONE
};

enum class PropID : int {
	NONE
};

union NodeIDMeta {
	unsigned char act_categ;
	int id;
};
