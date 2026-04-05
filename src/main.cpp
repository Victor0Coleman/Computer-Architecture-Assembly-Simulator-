#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Simulator.h"
#include "Assembler.h"

int main(int argc, char* argv[]) {
    std::string binPath = "instructions.bin";

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

    Assembler s;
    s.assemble(argv[1], binPath);
    std::vector<uint32_t> instructions;

    std::ifstream bin_file(binPath, std::ios::binary);
    if (!bin_file.is_open()) {
        std::cerr << "Failed to open input file";
        return 1;
    }

    while(bin_file.good()){
        char memblock[4];
        while (bin_file.read(memblock, 4)){
            uint32_t a = 0;
            for(int i = 0; i < 4; ++i){
                a = (a<<8) | (static_cast<unsigned int>(memblock[i]) & 0xff);
            }

            instructions.push_back(a);
            // std::cout << "main: " << std::hex << a << "\n";      // old line for verifying binary file read properly
        }

    }


    // run the simulator
    Simulator sim(debug);
    sim.loadProgram(instructions);
    sim.run();

    return 0;
}