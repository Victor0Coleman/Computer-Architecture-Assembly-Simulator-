#include "Simulator.h"
#include "encoder.h"    // This include only need to exist while encoding is on the chip. Bad, but temporary. Hopefully ~
#include "controlUnit.h"
#include <iostream>
#include <sstream>

Simulator::Simulator(bool debugMode) : pc(0), debug(debugMode) {}

void Simulator::loadProgram(const std::vector<uint32_t>& instructions) {
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
        ifid.instruction = 0;
    }
}

void Simulator::stageID() {

    // Encoding has moved off the chip. Huzzah.

    // Decode
    // Filter out no-op outside of switch (just for less clutter)
    if(ifid.instruction == 0){
        idex = ID_EX();
        return;
    }

    // All values will be populated... not all will be used.    Choose which to use based on idex.type.
    idex.opcode = (ifid.instruction >> 26) & 0x3F;
    idex.rs     = (ifid.instruction >> 21) & 0x1F;
    idex.rt     = (ifid.instruction >> 16) & 0x1F;
    idex.rd     = (ifid.instruction >> 11) & 0x1F;
    idex.shamt  = (ifid.instruction >> 06) & 0x1F;
    idex.funct  = (ifid.instruction >> 00) & 0x3F;
    
    int16_t intermediate_immediate = ifid.instruction & 0x0000FFFF;
    idex.immediate = (int32_t)intermediate_immediate; // auto extend?
    idex.jump_offset = ifid.instruction & 0x03FFFFFF;

    idex.readData1 = regFile.read(idex.rs);
    idex.readData2 = regFile.read(idex.rt);

    idex.type
        = idex.opcode == 0 ? idex.R
        : idex.opcode == 2 ? idex.J
        : idex.I;  // NOP filtered out already. However, if NOP was read as R-type, it would make no difference to correctness...probably?
    

    // Here will be assigning values for signals based on relevant codes. 
    // TODO.
    ControlUnit::setsignals(idex);

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