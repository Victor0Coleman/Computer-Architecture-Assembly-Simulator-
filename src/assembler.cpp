#include "assembler.h"
#include "encoder.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>    // label table


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


// do a bunch of crap that makes sure this is a valid line of machine code
// this one is hard and strict
// bad code = error (unlike encode)
static void validate(const std::string& line, int pc){
    return;
}


bool Assembler::assemble(const std::string& inputPath,
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

    // PASS TWO: convert instructions to machine code
    for (const std::string& line : pruned) {
        validate(line, pc);
        uint32_t instruction = encode(line, labelTable, pc);

        // 0x01234567 -> 01 23 45 67
        uint8_t bytes[4];
        bytes[0] = (instruction >> 24) & 0xFF;
        bytes[1] = (instruction >> 16) & 0xFF;
        bytes[2] = (instruction >> 8)  & 0xFF;
        bytes[3] = instruction & 0xFF;

        out.write(reinterpret_cast<char*>(bytes), 4);

        pc += 4;    // will enforce this
    }

    return true;
}

