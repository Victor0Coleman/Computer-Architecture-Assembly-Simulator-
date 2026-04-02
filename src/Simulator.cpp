#include "Simulator.h"
#include <iostream>
#include <sstream>

Simulator::Simulator(bool debugMode) : pc(0), debug(debugMode) {}

void Simulator::loadProgram(const std::vector<std::string>& instructions) {
    program = instructions;
}

void Simulator::run() {
    // keep running until pc has passed all instructions
    // and the pipeline is drained (no instruction in any stage)
    int totalCycles = program.size() + 4; // 4 extra cycles to drain pipeline

    for (int cycle = 1; cycle <= totalCycles; cycle++) {
        if (debug) {
            std::cout << "\n========== CYCLE " << cycle << " ==========\n";
        }

        // stages run backwards to avoid overwriting data
        // that later stages still need
        stageWB();
        stageMEM();
        stageEX();
        stageID();
        stageIF();

        if (debug) {
            printState();
        }
    }

    // final output — always printed regardless of debug mode
    regFile.dump();
    memory.dump();
}

// ─── STAGE STUBS ─────────────────────────────────────
// These are empty for now — your teammates will fill them in
// Each one is its own branch/ticket for the other team members

void Simulator::stageIF() {
    if (pc < (int)program.size()) {
        ifid.instruction = program[pc];
        ifid.pc = pc;
        pc++;
    } else {
        ifid.instruction = "NOP";
    }
}

void Simulator::stageID() {
    // Person 2's job — decode ifid.instruction
    // and fill in idex fields
}

void Simulator::stageEX() {
    // Person 3's job — read idex fields
    // run ALU and fill in exmem fields
}

void Simulator::stageMEM() {
    // Person 4's job — read exmem fields
    // access memory if needed and fill in memwb fields
}

void Simulator::stageWB() {
    // Person 4's job — read memwb fields
    // write result back to register file
}

void Simulator::printState() const {
    std::cout << "\n--- Pipeline Register State ---\n";
    std::cout << "IF/ID:  " << ifid.instruction << " (PC=" << ifid.pc << ")\n";
    std::cout << "ID/EX:  " << idex.opcode
              << " rs=" << idex.rs
              << " rt=" << idex.rt
              << " rd=" << idex.rd << "\n";
    std::cout << "EX/MEM: aluResult=" << exmem.aluResult
              << " destReg=" << exmem.destReg << "\n";
    std::cout << "MEM/WB: aluResult=" << memwb.aluResult
              << " readData=" << memwb.readData
              << " destReg=" << memwb.destReg << "\n";
    regFile.dump();
}