#pragma once

enum TileID : int {
	NONE,
	WALL,
	DOOR,
	WATER,
	STONEFLOOR
};

enum NodeCategory : unsigned char {
	NONE,
	PRIMARY_ACTION,
};

enum NodeID : int {
	NONE,
	ACTION,
	ANIMATION
};

enum AttrID : int {
	NONE
};

enum PropID : int {
	NONE
};

union NodeIDMeta {
	unsigned char category;
	int id;
};
