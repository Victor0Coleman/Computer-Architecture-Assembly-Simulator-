#include <string>

class Assembler {
public:
    // input: assembly code file
    // output: create/overwrite file with machine code 
    static bool assemble(const std::string& inputPath,
                         const std::string& outputPath);

};