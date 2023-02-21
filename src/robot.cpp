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

static inline void const printarr(const house& model, const std::pair<int, int> p, const std::pair<int, int> s, int currcharge = 0, int currsteps = 0) {
	cout << "Charge remaining: " << currcharge << " | Steps taken: " << currsteps << endl;
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

/*
* Gets the remaining battery on this robot.
*/
size_t Robot::remaining_battery() const {
	return current_battery;
}

/*
* Gets the max battery on this robot.
*/
size_t Robot::full_battery() const {
	return max_battery;
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
	bool asd, ase, asf;
	switch (direction) {
	case Direction::WEST:
		/*asd = !inbounds(current_row, current_col - 1);
		ase = model[current_row][current_col - 1] == sym_char(Symbol::WALL);
		asf = model[current_row][current_col - 1] == sym_char(Symbol::NONE);
		cout << "West" << asd << ase << asf << endl;*/
		return !inbounds(current_row, current_col - 1)
		|| model[current_row][current_col - 1] == sym_char(Symbol::WALL)
		|| model[current_row][current_col - 1] == sym_char(Symbol::NONE);
	case Direction::EAST:
		/*asd = !inbounds(current_row, current_col + 1);
		ase = model[current_row][current_col + 1] == sym_char(Symbol::WALL);
		asf = model[current_row][current_col + 1] == sym_char(Symbol::NONE);
		cout << "East" << asd << ase << asf << endl;*/
		return !inbounds(current_row, current_col + 1)
		|| model[current_row][current_col + 1] == sym_char(Symbol::WALL)
		|| model[current_row][current_col + 1] == sym_char(Symbol::NONE);
	case Direction::SOUTH:
		/*asd = !inbounds(current_row + 1, current_col);
		ase = model[current_row + 1][current_col] == sym_char(Symbol::WALL);
		asf = model[current_row + 1][current_col] == sym_char(Symbol::NONE);
		cout << "Soup" << asd << ase << asf << endl;*/
		return !inbounds(current_row + 1, current_col)
		|| model[current_row + 1][current_col] == sym_char(Symbol::WALL)
		|| model[current_row + 1][current_col] == sym_char(Symbol::NONE);
	case Direction::NORTH: 
		/*asd = !inbounds(current_row - 1, current_col);
		ase = model[current_row - 1][current_col] == sym_char(Symbol::WALL);
		asf = model[current_row - 1][current_col] == sym_char(Symbol::NONE);
		cout << "nerRth" << asd << ase << asf << endl;*/
		return !inbounds(current_row - 1, current_col)
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
		--current_battery;
		Direction dir = controller->get_next_step();
		//cout << static_cast<int> (dir) << endl;
		switch (dir) {
		case Direction::NORTH:
			cout << "Going North" << endl;
			if (!is_wall(dir)) {
				--current_row;
				printarr(model, std::pair<int, int>(current_col, current_row), std::pair<int, int>(charge_col, charge_row), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::SOUTH:
			cout << "Going South" << endl;
			if (!is_wall(dir)) {
				++current_row;
				printarr(model, std::pair<int, int>(current_col, current_row), std::pair<int, int>(charge_col, charge_row), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::EAST:
			cout << "Going East" << endl;
			if (!is_wall(dir)) {
				++current_col;
				printarr(model, std::pair<int, int>(current_col, current_row), std::pair<int, int>(charge_col, charge_row), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::WEST:
			cout << "Going West" << endl;
			if (!is_wall(dir)) {
				--current_col;
				printarr(model, std::pair<int, int>(current_col, current_row), std::pair<int, int>(charge_col, charge_row), current_battery, current_steps);
			}
			else {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::STAY:
			cout << "Staying Still" << endl;
			if (current_row == charge_row && current_col == charge_col) 
				current_battery = std::min(current_battery + max_battery / 20, max_battery);
			else --model[current_row][current_col];
			break;
		default:
			cout << "Uhhhh NONE" << endl;
			break;
		}
		sleep_for(milliseconds(500));
	}
	return 1;
}

inline bool Robot::inbounds(int x, int y) const {
	//cout << "Inbounds check " << x << y << endl;
	return !(x < 0 || y < 0 || x >= model[0].size() || y >= model.size());
}
size_t Robot::calculate_dirt(const house& model) const {
	size_t cnt = 0;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cnt += std::max(0, static_cast<int>(model[i][j] - '0'));
		}
	}
	return cnt;
}