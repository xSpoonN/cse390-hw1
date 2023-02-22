#ifndef DIRECTIONS
#define DIRECTIONS

enum class Direction { NONE, NORTH, EAST, SOUTH, WEST, STAY };

inline std::string dirstr(Direction dir) {
	switch (dir) {
		case Direction::NONE: return "None";
		case Direction::NORTH: return "North";
		case Direction::EAST: return "East";
		case Direction::SOUTH: return "South";
		case Direction::WEST: return "West";
		case Direction::STAY: return "Stay";
	}
}

#endif