#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define err(msg) cout << msg << endl; file.close(); return EXIT_FAILURE;

using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;
using std::max;
using std::min;

void const printarr(const vector<vector<char>>& model, const std::pair<int, int> p, int currcharge = 0, int currsteps = 0) {
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
	std::ifstream file(argv[1]);
	if (!file.is_open()) { err("Invalid File given"); }

	/* Parse Max Charge */
	string line; int charge = 0; int steps = 0; int pos;
	if (getline(file, line)) {
		pos = line.find("MAX_CHARGE:");
		if (pos == string::npos) { err("Maximum Charge not defined!"); }
		string mcin = line.substr(pos + 12);
		for (auto c : mcin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_CHARGE input!"); }
		}
		charge = std::stoi(mcin);
	} else { err("Maximum Charge not defined!"); }

	/* Parse Max Steps */
	if (getline(file, line)) {
		pos = line.find("MAX_STEPS:");
		if (pos == string::npos) { err("Maximum Steps not defined!"); }
		string msin = line.substr(pos + 11);
		for (auto c : msin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_STEPS input!"); }
		}
		steps = std::stoi(msin);
	} else { err("Maximum Steps not defined!"); }

	/* Read input into a matrix */
	vector<vector<char>> model; int row = 0; int col = 0;
	std::pair<int, int> s(-1,-1); /// Start Position
	while (getline(file, line)) {
		vector<char> rowvec;
		for (auto c : line) {
			rowvec.push_back(c);
			if (c == '+') {
				if (s.first == -1 && s.second == -1) s = std::make_pair(row, col);
				else { err("Only one start position may be defined!"); }
			}
			++col;
		}
		model.push_back(rowvec);
		++row; col = 0;
	}
	printarr(model, s, charge, steps);
	cout << s.first << ", " << s.second << endl;

	/* Simulate the Roomba*/
	std::pair<int, int> p = s; /// Represents the position of the Roomba.
	int dirtarr[5] = {-1, -1, -1, -1, -1}; /// Dirt levels of {Center, Top, Right, Bottom, Left}
	while (true) {
		dirtarr[0] = model[p.second][p.first] - '0'; // Center
		dirtarr[1] = model[max(0, p.second - 1)][p.first] - '0'; // Top
		dirtarr[2] = model[p.second][min(int(model[0].size() - 1), p.first + 1)] - '0'; // Right
		dirtarr[3] = model[min(int(model.size() - 1), p.second + 1)][p.first] - '0'; // Bottom
		dirtarr[4] = model[p.second][max(0, p.first - 1)] - '0'; // Left
		int* maxdirt = std::max_element(dirtarr, dirtarr + 5);
		if (*maxdirt < 0) {
			int* findplus = std::find(dirtarr + 1, dirtarr + 5, -5); /* Only check last 4 values, since its possible center is +. */
			if (findplus == dirtarr + 5) {
				 /* Plus not found, error cause roomba is surrounded by walls mikeyDoofus */ 

			}
			model[p.second][p.first] = -2; /* Set this position to a wall internally, so the algo won't revisit this dead end. */
			maxdirt = findplus;
		}
		switch (std::distance(dirtarr, maxdirt)) {
		case 1: --p.second; 
			--charge; --steps; 
			cout << "Moving up." << endl; 
			break;
		case 2: ++p.first; 
			--charge; --steps; 
			cout << "Moving right." << endl; 
			break;
		case 3: ++p.second; 
			--charge; --steps; 
			cout << "Moving down." << endl; 
			break;
		case 4: --p.first; 
			--charge; --steps; 
			cout << "Moving left." << endl; 
			break;
		case 0: default: cout << "Not moving. Dirt Level: " << model[p.second][p.first] << endl; break;
		}
		/* Check battery/step stuff */


		model[p.second][p.first] = max('0', static_cast<char>(model[p.second][p.first] - 1)); /* Decrement this tile's dirt level unless it's already 0. */
		printarr(model, p, charge, steps);
	}

	return EXIT_SUCCESS;
}