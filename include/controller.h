#ifndef CONTROLLER
#define CONTROLLER

#include "directions.h"

class Controller {
public:
	Controller();
	Direction get_next_step(const Robot&);
};

#endif