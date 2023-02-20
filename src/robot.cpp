#include "robot.h"
#include "controller.h"

class Robot {
	house model;
	Controller controller;
	size_t current_battery;
	size_t max_battery;
public:
	Robot() {

	}
	int clean_house() {

	}
	int get_dirt() {

	}
	int is_wall(Direction direction) {

	}
	int remaining_battery() {
		return current_battery;
	}
};
