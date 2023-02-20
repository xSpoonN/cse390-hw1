#ifndef CONTROLLER
#define CONTROLLER

#include "directions.h"
#include "robot.h"

class Controller {
	const Robot& robot;
public:
	Controller(const Robot&);
	Direction get_next_step();
};

#endif