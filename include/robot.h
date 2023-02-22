#ifndef ROBOT
#define ROBOT

#include <vector>

#include "directions.h"

using house = std::vector<std::vector<char>>;

class Controller;

class Robot {
	// Robot Data
	Controller* controller;
	size_t current_battery;
	size_t max_battery;
	size_t current_steps;
	size_t max_steps;

	// House Metadata
	house& model;
	size_t remaining_dirt;
	int current_row;
	int current_col;
	// Todo: remove these
	int charge_row;
	int charge_col;

	// Helper Methods
	bool inbounds(int, int) const;
	size_t calculate_dirt(const house&) const;
public:
	Robot(house& model, size_t max_battery, size_t max_steps, int starting_row, int starting_col);
	~Robot();
	int clean_house();
	int get_dirt_underneath() const;
	bool is_wall(Direction) const;
	size_t remaining_battery() const;
};

#endif