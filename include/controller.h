#ifndef CONTROLLER
#define CONTROLLER

#include "directions.h"
#include "robot.h"

class Controller {
	const Robot& rob;
	size_t steps_from_charger;
	vector<Direction> path_to_charger;

	Direction naive_algorithm();
public:
	Controller(const Robot&);
	Direction get_next_step();
};

#endif