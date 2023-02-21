#include <iostream>
#include <vector>

#include "robot.h"
#include "controller.h"
#include "symbols.h"

using std::cout;
using std::endl;
using std::vector;

static inline void const printarr(const house& model, const std::pair<int, int> p, int currcharge = 0, int currsteps = 0) {
	cout << "Charge remaining: " << currcharge << " | Steps remaining: " << currsteps << endl;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cout << ((p.second == i && p.first == j) ? 'x' : model[i][j]) << " ";
		}
		cout << endl;
	}
}


Robot::Robot(house& model, size_t max_battery, size_t max_steps, int starting_row, int starting_col)
	: controller(new Controller(this)), current_battery(max_battery), max_battery(max_battery), current_steps(0), max_steps(max_steps), model(model)
	, remaining_dirt(calculate_dirt(model)), current_row(starting_row), current_col(starting_col) {}

/*
* Gets the remaining battery on this robot.
*/
size_t Robot::remaining_battery() const {
	return current_battery;
}

/*
* Gets the amount of dirt this robot is currently sitting on.
*/
size_t Robot::get_dirt_underneath() const {
	switch (model[current_row][current_col]) {
	case sym_char(Symbol::DIRT1): return 1;
	case sym_char(Symbol::DIRT2): return 2;
	case sym_char(Symbol::DIRT3): return 3;
	case sym_char(Symbol::DIRT4): return 4;
	case sym_char(Symbol::DIRT5): return 5;
	case sym_char(Symbol::DIRT6): return 6;
	case sym_char(Symbol::DIRT7): return 7;
	case sym_char(Symbol::DIRT8): return 8;
	case sym_char(Symbol::DIRT9): return 9;
	default: return 0;
	}
}

/*
* Checks if there is a wall in the given direction relative to the robot.
*/
bool Robot::is_wall(Direction direction) const {
	switch (direction) {
	case Direction::WEST: return !inbounds(current_row, current_col - 1)
		|| model[current_row][current_col - 1] == sym_char(Symbol::WALL)
		|| model[current_row][current_col - 1] == sym_char(Symbol::NONE);
	case Direction::EAST: return !inbounds(current_row, current_col + 1)
		|| model[current_row][current_col + 1] == sym_char(Symbol::WALL)
		|| model[current_row][current_col + 1] == sym_char(Symbol::NONE);
	case Direction::SOUTH: return !inbounds(current_row + 1, current_col)
		|| model[current_row + 1][current_col] == sym_char(Symbol::WALL)
		|| model[current_row + 1][current_col] == sym_char(Symbol::NONE);
	case Direction::NORTH: return !inbounds(current_row - 1, current_col)
		|| model[current_row - 1][current_col] == sym_char(Symbol::WALL)
		|| model[current_row - 1][current_col] == sym_char(Symbol::NONE);
	default:
		return true;
	}
}

/*
* Attempts to clean the house this robot was initialized with.
*/
int Robot::clean_house() {
	while (current_steps < max_steps && remaining_dirt > 0 && current_battery > 0) {
		++current_steps;
		Direction dir = controller->get_next_step();
		switch (dir) {
		case Direction::NORTH:
			if (!is_wall(dir)) {
				++current_row;
				printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::SOUTH:
			if (!is_wall(dir)) {
				--current_row;
				printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::EAST:
			if (!is_wall(dir)) {
				++current_col;
				printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::WEST:
			if (!is_wall(dir)) {
				--current_col;
				printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::STAY:
			if (!is_wall(dir)) {
				printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		default:
			cout << "Uhhhh NONE" << endl;
			break;
		}
	}
	return 1;
}

bool Robot::inbounds(int x, int y) const {
	return x < 0 || y < 0 || x >= model[0].size() || y >= model.size();
}
size_t Robot::calculate_dirt(const house& model) const {
	// Todo: implement
	return 10;
}