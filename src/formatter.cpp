#include "formatter.h"
#include "encoder.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>    // label table
#include <sstream>


// helpful cleanup functions contributed by ChatGPT
static std::string _strip_comments(const std::string& line) {
    size_t pos = line.find('#');
    return (pos == std::string::npos) ? line : line.substr(0, pos);
}

static std::string _trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


bool Formatter::generate(const std::string& inputPath,
                         const std::string& outputPath) {
    std::ifstream in(inputPath);
    if (!in.is_open()) {
        std::cerr << "Failed to open input file";
        return false;
    }

    std::ofstream out(outputPath);
    if (!out.is_open()) {
        std::cerr << "Failed to open output file";
        return false;
    }

    std::unordered_map<std::string, int> labelTable;
    std::string line;
    int pc = 0;

    // PASS ONE: prune file
    std::vector<std::string> pruned;

    while (std::getline(in, line)) {
        line = _strip_comments(line);
        line = _trim(line);

        if (line.empty()) 
            continue;

        // label detection "LABEL:"
        std::size_t colon = line.find(':');
        if (colon != std::string::npos) {   // colon != npos -> a label is found

            std::string label = _trim(line.substr(0, colon));

            if (labelTable.count(label)) {  
                std::cerr << "error: label " << label << " appears more than once!";
                return false;
            }

            labelTable[label] = pc;

            // Remove label from line
            line = _trim(line.substr(colon + 1));

            if (line.empty()) continue; // label-only line
        }

        pruned.push_back(line);
        pc += 4;
    }

    pc = 0;

    // PASS 2 (format output)
    for (const std::string& instr : pruned) {
        uint32_t encoded = encode(instr, labelTable, pc);

        // hex
        std::stringstream hexStream;
        hexStream << "0x"
                  << std::uppercase
                  << std::hex
                  << std::setw(8)
                  << std::setfill('0')
                  << encoded;

        // binary
        std::string binary = "";
        for (int i = 31; i >= 0; --i) {
            binary += ((encoded >> i) & 1) ? '1' : '0';
        }

        out << std::left << std::setw(30) << instr
            << std::setw(12) << hexStream.str()
            << binary << "\n";

        pc += 4;
    }

    return true;
}