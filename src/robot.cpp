#include <iostream>
#include <fstream>
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

static bool debug = false;

static inline void const printarr(const house& model, const std::pair<int, int> p, int currcharge = 0, int currsteps = 0, int maxsteps = -1, int maxcharge = -1) {
	cout << "Charge remaining: " << currcharge;
	if (maxcharge >= 0) cout << "/" << maxcharge;
	cout << " | Steps taken: " << currsteps;
	if (maxsteps >= 0) cout << "/" << maxsteps;
	cout << endl;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cout << ((p.first == i && p.second == j) ? 'x' : model[i][j]) << " ";
		}
		cout << endl;
	}
}


Robot::Robot(house& model, int max_battery, int max_steps, int starting_row, int starting_col)
	: current_battery(max_battery), max_battery(max_battery), current_steps(0), max_steps(max_steps), controller(new Controller(this))
	, model(model), remaining_dirt(calculate_dirt()), current_row(starting_row), current_col(starting_col) {}

Robot::~Robot() {
	delete controller;
}

/*
* Gets the remaining battery on this robot.
*/
int Robot::remaining_battery() const {
	return current_battery;
}

/*
* Gets the amount of dirt this robot is currently sitting on.
*/
int Robot::get_dirt_underneath() const {
	int dirt = Sym::get_dirt_level(model[current_row][current_col]);
	return dirt > 0 ? dirt : 0;
}

/*
* Checks if there is a wall in the given direction relative to the robot.
*/
bool Robot::is_wall(Direction direction) const {
	switch (direction) {
	case Direction::WEST:
		//cout << "West: ";
		return /*!inbounds(current_row, current_col - 1) ||*/ Sym::is_wall(model[current_row][current_col - 1]);
	case Direction::EAST:
		//cout << "East: ";
		return /*!inbounds(current_row, current_col + 1) ||*/ Sym::is_wall(model[current_row][current_col + 1]);
	case Direction::SOUTH:
		//cout << "South: ";
		return /*!inbounds(current_row + 1, current_col) ||*/ Sym::is_wall(model[current_row + 1][current_col]);
	case Direction::NORTH:
		//cout << "North: ";
		return /*!inbounds(current_row - 1, current_col) ||*/ Sym::is_wall(model[current_row - 1][current_col]);
	default:
		return true;
	}
}

/*
* Attempts to clean the house this robot was initialized with.
* 
* @return the amount of steps taken
*/
int Robot::clean_house(std::ofstream& output_file) {
	while (current_steps < max_steps && current_battery > 0) {
		--current_battery;
		++current_steps;
		/* Update current position based on direction received */
		Direction dir = controller->get_next_step();
		if (debug) cout << "Direction: " << dirstr(dir) << endl;
		output_file << "Step " << current_steps << ": " << dirstr(dir);
		switch (dir) {
		case Direction::NORTH:
			if (!is_wall(dir)) {
				--current_row;
			}
			else if(debug) {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::SOUTH:
			if (!is_wall(dir)) {
				++current_row;
			}
			else if (debug) {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::EAST:
			if (!is_wall(dir)) {
				++current_col;
			}
			else if (debug) {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::WEST:
			if (!is_wall(dir)) {
				--current_col;
			}
			else if (debug) {
				cout << "Controller tried to direct us into a wall!" << endl;
			}
			break;
		case Direction::STAY:
			if (Sym::get_dirt_level(model[current_row][current_col]) > 0) {
				output_file << " | Cleaning dirt level " << Sym::get_dirt_level(model[current_row][current_col]);
				if (debug) cout << "Cleaning..." << endl;
				Sym::decrement_dirt(model[current_row][current_col]);
				--remaining_dirt;
			}
			else if (model[current_row][current_col] != Sym::CHARGER) {
				if (debug) cout << "Told to stay still without cleaning/charging!" << endl;
			}
			break;
		}
		/* If at the dock, begin charging */
		if (model[current_row][current_col] == Sym::CHARGER) {  // Todo: add charger_dist?
			if (debug) cout << "Charging..." << endl;
			output_file << " | Charging (" << current_battery << "/" << max_battery << ") -> (";
			current_battery = std::min(current_battery + (max_battery / 20) + 1, max_battery);
			output_file << current_battery << "/" << max_battery << ")";
			if (remaining_dirt == 0) {
				output_file << endl;
				break;
			}
		}
		output_file << endl;
		/* Print current matrix to console */
		if (debug) {
			printarr(model, std::pair<int, int>(current_row, current_col), current_battery, current_steps, max_steps, max_battery);
			sleep_for(milliseconds(400));
		}
	}
	output_file << "Total steps: " << current_steps << endl;
	output_file << "Dirt left: " << calculate_dirt() << endl;
	output_file << "Dead battery: " << (current_battery == 0 ? "True" : "False") << endl;
	output_file << "Mission success: " << (calculate_dirt() == 0 && model[current_row][current_col] == Sym::CHARGER ? "True" : "False") << endl;
	return current_steps;
}

int Robot::calculate_dirt() const {
	int cnt = 0;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			int dirt = Sym::get_dirt_level(model[i][j]);
			cnt += dirt > 0 ? dirt : 0;
		}
	}
	return cnt;
}