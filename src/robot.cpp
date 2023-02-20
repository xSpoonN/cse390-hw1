#include "robot.h"
#include "controller.h"

class Robot {
	// Robot Data
	Controller controller;
	size_t current_battery;
	size_t max_battery;
	size_t current_steps;
	size_t max_steps;

	// House Metadata
	house& model;
	size_t remaining_dirt;
	size_t current_row;
	size_t current_col;
public:
	Robot::Robot(house& model, size_t max_battery, size_t max_steps)
		: controller(*this), current_battery(max_battery), max_battery(max_battery), current_steps(0), max_steps(max_steps), model(model), remaining_dirt(calculate_dirt(model)) {
		setup_start_position();
	}

	/*
	* Gets the remaining battery on this robot.
	*/
	size_t remaining_battery() const {
		return current_battery;
	}

	/*
	* Gets the amount of dirt this robot is currently sitting on.
	*/
	size_t get_dirt_underneath() const {
		// Todo: implement
		return 0;
	}

	/*
	* Checks if there is a wall in the given direction relative to the robot.
	*/
	bool is_wall(Direction direction) const {
		// Todo: implement
		return false;
	}

	/*
	* Attempts to clean the house this robot was initialized with.
	*/
	int clean_house() {
		while (current_steps < max_steps && remaining_dirt > 0 && current_battery > 0) {
			++current_steps;
			Direction dir = controller.get_next_step();
			// Todo: the rest of the program
		}
	}
private:  // Helper functions
	size_t calculate_dirt(const house& model) {
		// Todo: implement
		return 10;
	}
	inline void setup_start_position() {

	}
};
