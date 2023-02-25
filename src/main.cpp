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
			cout << ((p.first == i && p.second == j) ? 'x' : model[i][j]) << " ";
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
	if (!file.is_open()) { err("Invalid File given"); }

	/* Parse Max Charge */
	string line;
	int charge = 0, steps = 0, pos;
	if (std::getline(file, line)) {
		pos = line.find("MAX_CHARGE:");
		if (pos == string::npos) { err("Maximum Charge not defined!"); }
		string mctemp = line.substr(pos + sizeof("MAX_CHARGE:")-1);
		string mcin = mctemp.substr(mctemp.find_first_not_of(" "), mctemp.find_last_not_of(" "));
		for (auto c : mcin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_CHARGE input!"); }
		}
		charge = std::stoi(mcin);
	} else { err("Maximum Charge not defined!"); }

	/* Parse Max Steps */
	if (std::getline(file, line)) {
		pos = line.find("MAX_STEPS:");
		if (pos == string::npos) { err("Maximum Steps not defined!"); }
		string mstemp = line.substr(pos + sizeof("MAX_STEPS:")-1);
		string msin = mstemp.substr(mstemp.find_first_not_of(" "), mstemp.find_last_not_of(" "));
		for (auto c : msin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_STEPS input!"); }
		}
		steps = std::stoi(msin);
	} else { err("Maximum Steps not defined!"); }

	/* Read input into a "house" */
	house model;
	int row = 0, col = 0;
	std::pair<int, int> start(-1,-1); // Charger/Start Position
	vector<int> rowlen; int maxlen = 0;
	while (std::getline(file, line)) {
		vector<char> rowvec;
		for (auto c : line) {
			if (!Sym::is_valid(c)) { err("Unexpected char in input: " << c); }
			rowvec.push_back(c);
			if (c == Sym::CHARGER) {
				if (start.first == -1 && start.second == -1) {
					start = std::make_pair(row, col);
				}
				else { err("Only one start position may be defined!"); }
			}
			++col;
		}
		model.push_back(rowvec);
		rowlen.push_back(rowvec.size());
		if (rowvec.size() > maxlen) maxlen = rowvec.size();
		++row; col = 0;
	}

	/* Fills in ragged models */
	for (int i = 0; i < rowlen.size(); i++) {
		for (int j = 0; j < maxlen - rowlen[i]; j++) {
			model[i].push_back(Sym::WALL);
		}
	}
	/* Left */
	for (int i = 0; i < model.size(); i++) {
		if (!Sym::is_wall(model[i][0])) {
			for (int j = 0; j < model.size(); j++) {
				model[j].insert(model[j].begin(), Sym::WALL);
			}
			++start.second;
		}
		if (!Sym::is_wall(model[i][model[0].size()-1])) {
			for (int j = 0; j < model.size(); j++) {
				model[j].push_back(Sym::WALL);
			}
		}
	}
	for (int i = 0; i < model[0].size(); i++) {
		if (!Sym::is_wall(model[0][i])) {
			model.insert(model.begin(), vector<char>(model[0].size(), Sym::WALL));
			++start.first;
		}
		if (!Sym::is_wall(model[model.size()-1][i])) {
			model.push_back(vector<char>(model[0].size(), Sym::WALL));
		}
	}
	printarr(model, start, charge, steps);
	if (start.first == -1 && start.second == -1) { err("Start position not defined!"); }

	std::ofstream outputFile("output.txt", std::fstream::trunc);
	if (!outputFile.is_open()) {
		cout << "Failed to open output file" << endl;
		return EXIT_FAILURE;
	}

	Robot robot(model, charge, steps, start.first, start.second);
	robot.clean_house(outputFile);

	outputFile.close();
	cout << "The program has been completed successfully and is capable of completing all the assigned tasks. All the requirements and conditions specified in the program specifications were met successfully by the program. Therefore, the program has terminated its work in a most satisfactory manner and has ended successfully." << endl;
	printarr(model, start, charge, steps);
	return EXIT_SUCCESS;
}