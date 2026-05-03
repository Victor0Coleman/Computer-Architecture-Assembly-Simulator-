#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Simulator.h"
#include "assembler.h"
#include "formatter.h"

int main(int argc, char* argv[]) {
    std::string binPath = "instructions.bin";
    std::string formatterDirPath = "input_fancy";

    if (argc < 2) {
        std::cout << "Usage: ./mips-sim <input.asm> [-d]\n";
        return 1;
    }

    // check for debug flag
    bool debug = false;
    if (argc == 3 && std::string(argv[2]) == "-d") {
        debug = true;
    }

    std::string path = argv[1];

    // read the input file line by line
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error: could not open file " << path << "\n";
        return 1;
    }

    Assembler a;
    a.assemble(path, binPath);
    std::vector<uint32_t> instructions;


    std::string formatOutPath = formatterDirPath;

    {
        std::size_t slash = path.find_last_of("/\\");
        std::string filename = slash == std::string::npos ? path : path.substr(slash + 1); 
        
        std::size_t dot = filename.find_last_of('.');
        std::string filenameNoExtension = dot == std::string::npos ? filename : filename.substr(0, dot);

        formatOutPath += "/" + filenameNoExtension + ".txt"; 
    }

    Formatter f;
    f.generate(path, formatOutPath);

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
