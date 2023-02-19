#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#define err(msg) cout << msg << endl; file.close(); return EXIT_FAILURE;

using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::vector;

void const printarr(const vector<vector<char>>& model, int currcharge = 0, int currsteps = 0) {
	cout << "Charge remaining: " << currcharge << " | Steps remaining: " << currsteps << endl;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[0].size(); j++) {
			cout << model[i][j] << " ";
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
	string line; int maxcharge = 0; int maxsteps = 0; int pos;
	if (getline(file, line)) {
		pos = line.find("MAX_CHARGE:");
		if (pos == string::npos) { err("Maximum Charge not defined!"); }
		string mcin = line.substr(pos + 12);
		for (auto c : mcin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_CHARGE input!"); }
		}
		maxcharge = std::stoi(mcin);
	} else { err("Maximum Charge not defined!"); }

	/* Parse Max Steps */
	if (getline(file, line)) {
		pos = line.find("MAX_STEPS:");
		if (pos == string::npos) { err("Maximum Steps not defined!"); }
		string msin = line.substr(pos + 11);
		for (auto c : msin) { /* Validates input */
			if (!std::isdigit(c)) { err("Invalid MAX_STEPS input!"); }
		}
		maxsteps = std::stoi(msin);
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
	printarr(model, maxcharge, maxsteps);
	cout << s.first << ", " << s.second << endl;

	/* Simulate the Roomba*/


	return EXIT_SUCCESS;
}