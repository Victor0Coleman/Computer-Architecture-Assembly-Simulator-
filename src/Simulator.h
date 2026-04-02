#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <string>
#include "RegisterFile.h"
#include "Memory.h"
#include "PipelineRegisters.h"

class Simulator {
public:
    Simulator(bool debugMode);

    void loadProgram(const std::vector<std::string>& instructions);
    void run();

private:
    // the hardware components you already built
    RegisterFile regFile;
    Memory memory;

    // the four pipeline registers
    IF_ID   ifid;
    ID_EX   idex;
    EX_MEM  exmem;
    MEM_WB  memwb;

    // the program — list of instruction strings
    std::vector<std::string> program;

    // program counter — tracks which instruction to fetch next
    int pc;

    // debug mode flag
    bool debug;

    // the five stages — each one is its own function
    void stageIF();
    void stageID();
    void stageEX();
    void stageMEM();
    void stageWB();

    // debug helper
    void printState() const;
};

#endif