#ifndef DIRECTIONS
#define DIRECTIONS

enum class Direction { NONE, NORTH, EAST, SOUTH, WEST, STAY };

inline std::string dirstr(Direction dir) {
	switch (dir) {
	case Direction::NONE: return "NONE";
	case Direction::NORTH: return "NORTH";
	case Direction::EAST: return "EAST";
	case Direction::SOUTH: return "SOUTH";
	case Direction::WEST: return "WEST";
	case Direction::STAY: return "STAY";
	}
}

#endif