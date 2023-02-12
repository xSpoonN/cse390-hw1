#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

int main() {
	std::ifstream file;
	file.open("input.txt");
	std::string line; int maxcharge = 0; int maxsteps = 0;
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