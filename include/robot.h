#ifndef ROBOT
#define ROBOT

#include <vector>

#include "directions.h"

using house = std::vector<std::vector<char>>;

class Robot {
public:
	Robot(house, int, int);
	int clean_house();
	size_t get_dirt_underneath() const;
	bool is_wall(Direction direction) const;
	size_t remaining_battery() const;
};

#endif