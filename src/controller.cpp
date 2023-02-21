#include <vector>
#include <stack>
#include <iterator>
#include <random>
#include <string>
#include <algorithm>
#include <iostream>

#include "controller.h"
#include "robot.h"
#include "directions.h"

using std::vector;
using std::stack;
using std::pair;
using std::cout;
using std::endl;

Controller::Controller(const Robot* rob): rob(rob), steps_from_charger(0), path_to_charger({}), cnt(0) {}

Direction Controller::get_next_step() {
	if (rob->remaining_battery()-1 <= path_to_charger.size()) {
		cout << "Returning to Charger" << endl;
		cnt = 20; /* Robot takes 20 steps to fully charge */
		if (path_to_charger.size() == 0) {
			--cnt; return Direction::STAY;
		}
		Direction popped = path_to_charger.back();
		path_to_charger.pop_back();
		return popped;
	};
	if (cnt-- > 0) return Direction::STAY; /* Recharge the robot. */
	Direction dir = naive_algorithm();
	switch (dir) { /* Push the reverse into path_to_charger vec. */
	case Direction::NORTH:
		path_to_charger.push_back(Direction::SOUTH); break;
	case Direction::EAST:
		path_to_charger.push_back(Direction::WEST); break;
	case Direction::SOUTH:
		path_to_charger.push_back(Direction::NORTH); break;
	case Direction::WEST:
		path_to_charger.push_back(Direction::EAST); break;
	}
	++steps_from_charger;
	return dir;
}

Direction Controller::naive_algorithm() {
	vector<Direction> choice;
	if (rob->get_dirt_underneath() > 0) return Direction::STAY; /* If there's dirt stay still */
	if (!rob->is_wall(Direction::NORTH)) {
		/*cout << "North" << endl;*/
		choice.push_back(Direction::NORTH);
	}
	if (!rob->is_wall(Direction::EAST)) {
		//cout << "East" << endl;
		choice.push_back(Direction::EAST);
	}
	if (!rob->is_wall(Direction::SOUTH)) {
		//cout << "Sth" << endl;
		choice.push_back(Direction::SOUTH);
	}
	if (!rob->is_wall(Direction::WEST)) {
		//cout << "West" << endl;
		choice.push_back(Direction::WEST);
	}
	if (choice.size() == 0) return Direction::NONE; /* Robert is walled in. xdd */
	return choice[std::rand() % choice.size()];
}

//struct Position { int x, y; };
//class Node {
//public:
//	int dirt;
//	Position coords;
//	vector<Node*> neighbours;
//	Direction pre;
//	Node(Position coords, Direction pre = Direction::NONE, int dirt = -1) : coords(coords), pre(pre), dirt(dirt) {}
//
//	bool operator==(const Node& r) {
//		return coords.x == r.coords.x && coords.y == r.coords.y;
//	}
//
//	Position nCoords() { return Position{ coords.x, coords.y - 1 }; }
//	Position eCoords() { return Position{ coords.x + 1, coords.y }; }
//	Position sCoords() { return Position{ coords.x, coords.y + 1 }; }
//	Position wCoords() { return Position{ coords.x - 1, coords.y }; }
//};

//void dfs(Position pos, vector<Node>& visited) {
//
//	Node start(Position{ 0, 0 });
//	Node* c = &start;
//	c->dirt = rob.get_dirt_underneath();
//	vector<Direction> choice;
//	if (!rob.is_wall(Direction::NORTH)) {
//		c->neighbours.push_back(new Node(c->nCoords(), Direction::SOUTH));
//		choice.push_back(Direction::NORTH);
//	}
//	if (!rob.is_wall(Direction::EAST)) {
//		c->neighbours.push_back(new Node(c->eCoords(), Direction::WEST));
//		choice.push_back(Direction::EAST);
//	}
//	if (!rob.is_wall(Direction::SOUTH)) {
//		c->neighbours.push_back(new Node(c->sCoords(), Direction::NORTH));
//		choice.push_back(Direction::SOUTH);
//	}
//	if (!rob.is_wall(Direction::WEST)) {
//		c->neighbours.push_back(new Node(c->wCoords(), Direction::EAST));
//		choice.push_back(Direction::WEST);
//	}
//	Direction selectD = choice[std::rand() % choice.size()];
//
//	//Node curr(pair<int, int>(0, 0));
//
//	//curr.dirt = rob.get_dirt_underneath();
//	//bool n = !rob.is_wall(Direction::NORTH);
//	//bool e = !rob.is_wall(Direction::EAST);
//	//bool s = !rob.is_wall(Direction::SOUTH);
//	//bool w = !rob.is_wall(Direction::WEST);
//
//	//if (n) {
//	//	Node n1Node(curr.nCoords(), &curr, -1);
//	//	if (visited[n1Node]) {
//
//	//	}
//
//	//}
//	// aaaauuuugggghhhhhhhhhh
//
//}


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