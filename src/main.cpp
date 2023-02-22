#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "symbols.h"
#include "robot.h"

#define err(msg) cout << msg << endl; file.close(); return EXIT_FAILURE;

using std::cout;
using std::endl;
using std::string;
using std::vector;
using house = vector<vector<char>>;

static inline void const printarr(const house & model, const std::pair<int, int> p, int currcharge = 0, int currsteps = 0) {
	cout << "Charge remaining: " << currcharge << " | Steps remaining: " << currsteps << endl;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cout << ((p.second == i && p.first == j) ? 'x' : model[i][j]) << " ";
		}
		cout << endl;
	}
}

int main(int argc, char** argv) {
	/* Verify that a file was passed as an arg. */
	if (argc < 2) {
		cout << "Please specify a file! " << *argv << " <filename>" << endl;
		return EXIT_FAILURE;
	}

	/* Attempt to open the file. */
	std::ifstream file(*(argv+1));
	if (!file.is_open()) {
		err("Invalid File given");
	}

	/* Parse Max Charge */
	string line;
	int charge = 0, steps = 0, pos;
	if (std::getline(file, line)) {
		pos = line.find("MAX_CHARGE:");
		if (pos == string::npos) {
			err("Maximum Charge not defined!");
		}
		string mcin = line.substr(pos + 12);  // Todo: remove hardcoded 12
		for (auto c : mcin) { /* Validates input */
			if (!std::isdigit(c)) {
				err("Invalid MAX_CHARGE input!");
			}
		}
		charge = std::stoi(mcin);
	} else {
		err("Maximum Charge not defined!");
	}

	/* Parse Max Steps */
	if (std::getline(file, line)) {
		pos = line.find("MAX_STEPS:");
		if (pos == string::npos) {
			err("Maximum Steps not defined!");
		}
		string msin = line.substr(pos + 11);  // Todo: remove hardcoded 11
		for (auto c : msin) { /* Validates input */
			if (!std::isdigit(c)) {
				err("Invalid MAX_STEPS input!");
			}
		}
		steps = std::stoi(msin);
	} else {
		err("Maximum Steps not defined!");
	}

	/* Read input into a "house" */
	house model;
	int row = 0, col = 0;
	std::pair<int, int> start(-1,-1); // Charger/Start Position Todo: move this into robot.cpp
	while (std::getline(file, line)) {
		vector<char> rowvec;
		for (auto c : line) {
			rowvec.push_back(c);
			if (c == Sym::CHARGER) {
				if (start.first == -1 && start.second == -1)
					start = std::make_pair(row, col);
				else {
					err("Only one start position may be defined!");
				}
			}
			++col;
		}
		model.push_back(rowvec);
		++row; col = 0;
	}
	printarr(model, start, charge, steps);
	cout << start.first << ", " << start.second << endl;

	Robot robot(model, charge, steps, start.first, start.second);
	robot.clean_house();

	return EXIT_SUCCESS;
}