#ifndef ROBOT
#define ROBOT

#include <vector>

#include "directions.h"

using house = std::vector<std::vector<char>>;

class Robot {
public:
	Robot();
	int clean_house();
	int get_dirt();
	int is_wall(Direction direction);
	int remaining_battery();
};

#endif