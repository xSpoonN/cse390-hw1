#include <vector>

#include "controller.h"

using std::vector;

class Controller {
	size_t steps_from_charger;
	vector<Direction> path_to_charger;
public:
	Controller() {

	}

	Direction get_next_step() {

	}
};

///* Simulate the Roomba*/
//std::pair<int, int> p = s; /// Represents the position of the Roomba.
//int dirtarr[5] = { -1, -1, -1, -1, -1 }; /// Dirt levels of {Center, Top, Right, Bottom, Left}
//while (true) {
//	dirtarr[0] = model[p.second][p.first] - '0'; // Center
//	dirtarr[1] = model[max(0, p.second - 1)][p.first] - '0'; // Top
//	dirtarr[2] = model[p.second][min(int(model[0].size() - 1), p.first + 1)] - '0'; // Right
//	dirtarr[3] = model[min(int(model.size() - 1), p.second + 1)][p.first] - '0'; // Bottom
//	dirtarr[4] = model[p.second][max(0, p.first - 1)] - '0'; // Left
//	int* maxdirt = std::max_element(dirtarr, dirtarr + 5);
//	if (*maxdirt < 0) {
//		int* findplus = std::find(dirtarr + 1, dirtarr + 5, -5); /* Only check last 4 values, since its possible center is +. */
//		if (findplus == dirtarr + 5) {
//			/* Plus not found, error cause roomba is surrounded by walls mikeyDoofus */
//
//		}
//		model[p.second][p.first] = -2; /* Set this position to a wall internally, so the algo won't revisit this dead end. */
//		maxdirt = findplus;
//	}
//	switch (std::distance(dirtarr, maxdirt)) {
//	case 1: --p.second;
//		--charge; --steps;
//		cout << "Moving up." << endl;
//		break;
//	case 2: ++p.first;
//		--charge; --steps;
//		cout << "Moving right." << endl;
//		break;
//	case 3: ++p.second;
//		--charge; --steps;
//		cout << "Moving down." << endl;
//		break;
//	case 4: --p.first;
//		--charge; --steps;
//		cout << "Moving left." << endl;
//		break;
//	case 0: default: cout << "Not moving. Dirt Level: " << model[p.second][p.first] << endl; break;
//	}
//	/* Check battery/step stuff */
//
//
//	model[p.second][p.first] = max('0', static_cast<char>(model[p.second][p.first] - 1)); /* Decrement this tile's dirt level unless it's already 0. */
//	printarr(model, p, charge, steps);
//}