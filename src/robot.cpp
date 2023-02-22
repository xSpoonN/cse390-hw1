#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "robot.h"
#include "controller.h"
#include "symbols.h"

using std::cout;
using std::endl;
using std::vector;
using namespace std::this_thread;
using namespace std::chrono;

static inline void const printarr(const house& model, const std::pair<int, int> p, const std::pair<int, int> s, 
	int currcharge = 0, int currsteps = 0, int maxsteps = -1, int maxcharge = -1) {
	cout << "Charge remaining: " << currcharge;
	if (maxcharge >= 0) cout << "/" << maxcharge;
	cout << " | Steps taken: " << currsteps;
	if (maxsteps >= 0) cout << "/" << maxsteps;
	cout << endl;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cout << ((p.second == i && p.first == j) ? 'x' : ((s.second == i && s.first == j) ? '+' : model[i][j])) << " ";
		}
		cout << endl;
	}
}


Robot::Robot(house& model, size_t max_battery, size_t max_steps, int starting_row, int starting_col)
	: controller(new Controller(this)), current_battery(max_battery), max_battery(max_battery), current_steps(0), max_steps(max_steps), model(model)
	, remaining_dirt(calculate_dirt(model)), current_row(starting_row), current_col(starting_col), charge_row(starting_row), charge_col(starting_col) {}

Robot::~Robot() {
	delete controller;
}

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
	int dirt = Sym::get_dirt_level(model[current_row][current_col]);
	return dirt > 0 ? dirt : 0;
}

/*
* Checks if there is a wall in the given direction relative to the robot.
*/
bool Robot::is_wall(Direction direction) const {
	// Todo: cleanup or update debug statements
	//bool asd, ase, asf;
	switch (direction) {
	case Direction::WEST:
		/*asd = !inbounds(current_row, current_col - 1);
		ase = model[current_row][current_col - 1] == sym_char(Symbol::WALL);
		asf = model[current_row][current_col - 1] == sym_char(Symbol::NONE);
		cout << "West" << asd << ase << asf << endl;*/
		return !inbounds(current_row, current_col - 1) || Sym::is_wall(model[current_row][current_col - 1]);
	case Direction::EAST:
		/*asd = !inbounds(current_row, current_col + 1);
		ase = model[current_row][current_col + 1] == sym_char(Symbol::WALL);
		asf = model[current_row][current_col + 1] == sym_char(Symbol::NONE);
		cout << "East" << asd << ase << asf << endl;*/
		return !inbounds(current_row, current_col + 1) || Sym::is_wall(model[current_row][current_col + 1]);
	case Direction::SOUTH:
		/*asd = !inbounds(current_row + 1, current_col);
		ase = model[current_row + 1][current_col] == sym_char(Symbol::WALL);
		asf = model[current_row + 1][current_col] == sym_char(Symbol::NONE);
		cout << "Soup" << asd << ase << asf << endl;*/
		return !inbounds(current_row + 1, current_col) || Sym::is_wall(model[current_row + 1][current_col]);
	case Direction::NORTH: 
		/*asd = !inbounds(current_row - 1, current_col);
		ase = model[current_row - 1][current_col] == sym_char(Symbol::WALL);
		asf = model[current_row - 1][current_col] == sym_char(Symbol::NONE);
		cout << "nerRth" << asd << ase << asf << endl;*/
		return !inbounds(current_row - 1, current_col) || Sym::is_wall(model[current_row - 1][current_col]);
	default:
		return true;
	}
}

/*
* Attempts to clean the house this robot was initialized with.
*/
int Robot::clean_house() {
	while (remaining_dirt > 0 && current_steps++ < max_steps && current_battery-- > 0) {
		/* Update current position based on direction received */
		Direction dir = controller->get_next_step();
		cout << "Direction: " << dirstr(dir) << endl;
		switch (dir) {
		case Direction::NORTH:
			if (!is_wall(dir)) {
				--current_row;
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::SOUTH:
			if (!is_wall(dir)) {
				++current_row;
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::EAST:
			if (!is_wall(dir)) {
				++current_col;
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::WEST:
			if (!is_wall(dir)) {
				--current_col;
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::STAY:
			if (Sym::get_dirt_level(model[current_row][current_col]) > 0) {
				cout << "Cleaning..." << endl;
				Sym::decrement_dirt(model[current_row][current_col]);
			}
			else if (model[current_row][current_col] != Sym::CHARGER) {
				cout << "Told to stay still without cleaning/charging!!" << endl;
			}
			break;
		}
		/* If at the dock, begin charging */
		if (current_row == charge_row && current_col == charge_col) {
			cout << "Charging..." << endl;
			current_battery = std::min(current_battery + (max_battery / 20) + 1, max_battery);
		}
		/* Print current matrix to console */
		printarr(model, std::pair<int, int>(current_col, current_row), std::pair<int, int>(charge_col, charge_row),
			current_battery, current_steps, max_steps, max_battery);
		sleep_for(milliseconds(800));
	}
	return 1;
}

inline bool Robot::inbounds(int x, int y) const {
	return x >= 0 && y >= 0 && x < model[0].size() && y < model.size();
}

size_t Robot::calculate_dirt(const house& model) const {
	size_t cnt = 0;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			int dirt = Sym::get_dirt_level(model[i][j]);
			cnt += dirt > 0 ? dirt : 0;
		}
	}
	return cnt;
}