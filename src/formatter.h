#include <string>

class Formatter {
public:
    // input: assembly code file
    // output: create/overwrite beautiful text file 
    static bool generate(const std::string& inputPath,
                         const std::string& outputPath);

};