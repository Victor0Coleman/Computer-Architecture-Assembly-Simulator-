#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Simulator.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./mips-sim <input.asm> [-d]\n";
        return 1;
    }

    // check for debug flag
    bool debug = false;
    if (argc == 3 && std::string(argv[2]) == "-d") {
        debug = true;
    }

    // read the input file line by line
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error: could not open file " << argv[1] << "\n";
        return 1;
    }

    std::vector<std::string> instructions;
    std::string line;
    while (std::getline(file, line)) {
        // skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        instructions.push_back(line);
    }

    // run the simulator
    Simulator sim(debug);
    sim.loadProgram(instructions);
    sim.run();

    return 0;
}