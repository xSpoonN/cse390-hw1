#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

using std::cout;

int main(int argc, char **argv) {
	// Verify that a file was passed as an arg.
	if (argc < 2) {
		cout << "Please specify a file! " << *argv << " <filename>" << std::endl;
		return EXIT_FAILURE;
	}
	// Attempt to open the file.
	std::ifstream file(*(argv+1));
	if (!file.is_open()) {
		cout << "Invalid file." << std::endl;
		return EXIT_FAILURE;
	}
	// Todo: Parse lines
	if (std::getline(file, line)) {
		int pos = line.find("MAXCHARGE:");
		if (pos == std::string::npos) {
			std::cout << "Maximum Charge not defined!" << std::endl;
			file.close(); return 1;
		}
		std::string mcin = line.substr(pos + 1);
		//Iterate through string and check for invalid chars
		maxcharge = std::stoi(mcin);
	}
	//Repeat for maxsteps


	file.close();
}