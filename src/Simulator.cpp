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
    
    // commented out because there is no upper bound on #instructions
    // int totalCycles = program.size() + 4; // 4 extra cycles to drain pipeline

    int cycle = 0;

    for (;;) {
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

        // Loop end condition: Pipeline is empty and no more instructions in file
        bool pipelineEmpty
            = ifid.isEmpty()
            && idex.isEmpty()
            && exmem.isEmpty()
            && memwb.isEmpty();

        // emergency debug when a should-be-empty pipeline stage is stuck
        // std::cout << pipelineEmpty << " " << ifid.isEmpty()<<" "<<idex.isEmpty()<<" "<<exmem.isEmpty()<<" "<<memwb.isEmpty()<<" \n";

        if(pc / 4 >= program.size() && pipelineEmpty)
            break;

    }


    // final output — always printed regardless of debug mode
    regFile.dump();
    memory.dump();
}

// ─── STAGE STUBS ─────────────────────────────────────
// These are empty for now — your teammates will fill them in
// Each one is its own branch/ticket for the other team members

void Simulator::stageIF() {
    if (pc/4 < (int)program.size()) {
        ifid.instruction = program[pc/4];
        ifid.valid = true;
        ifid.pc = pc;
        pc += 4;
    } else {
        ifid.instruction = 0;
        ifid.valid = false;
    }
}

void Simulator::stageID() {
    idex.valid = ifid.valid;
    if(!idex.valid)
        return;

    // Encoding has moved off the chip. Huzzah.

    // Decode
    // Filter out no-op outside of switch (just for less clutter)
    if(ifid.instruction == 0){
        idex = ID_EX();
        idex.valid = true;  // NOP IS VALID!
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
    ControlUnit::setsignals(idex);


    // Special jump logic: Leak in the Pipeline ~ Interdimensional Rift 
    if(idex.opcode == 2){       // top 4 pc                 // word-addressing
        uint32_t nextPC = ((ifid.pc + 4) & 0xF0000000) | (idex.jump_offset << 2);
        pc = nextPC;
        idex.valid = false; // Don't want next stage to receive "valid" as this is done.

        // note. THIS ONLY WORKS because stageID() occurs before stageIF()
        // causing the next stageID() to receive the instruction at nextPC
        // if this EVER CHANGES, need to insert a NOP 
    }

    // signal monitor. Commented out.

/*//----- Print details of this pipeline stage -----

    std::cout 
        << "----------------------------" << "\n"
        << "instructionHex: " << std::hex << ifid.instruction << "\n"
        << std::dec // wont make much diff
        << "regDst: " << idex.regDst << "\n"
        << "aluSrc: " << idex.aluSrc << "\n"
        << "memRead: " << idex.memRead << "\n"
        << "memWrite: " << idex.memWrite << "\n"
        << "regWrite: " << idex.regWrite << "\n"
        << "memToReg: " << idex.memToReg << "\n"
        << "branch: " << idex.branch << "\n"
        << "ALUOp: " << (int)idex.ALUOp << "\n"      // this will just be some number (its enum)
        << "----------------------------" << "\n";
        
//*/

}

void Simulator::stageEX() {
    exmem.valid = idex.valid;
    if(!exmem.valid)
        return;

    // double check nop
    if (idex.type == ID_EX::NOP) {
        exmem.clear();
        exmem.valid = true;
        return;
    }

    // add alu args and result
    int operandA = idex.readData1;
    int operandB = idex.aluSrc ? idex.immediate : idex.readData2;
    
    ALUResult aluResult = alu.execute(idex.ALUOp, operandA, operandB);

    // store ALU outputs
    exmem.aluResult = aluResult.value;
    exmem.writeData = idex.readData2;   // used for SW

    // pass control signals forward
    exmem.memRead  = idex.memRead;
    exmem.memWrite = idex.memWrite;
    exmem.regWrite = idex.regWrite;
    exmem.memToReg = idex.memToReg;

    // choose destination register
    exmem.destReg = idex.regDst ? idex.rd : idex.rt;
}

void Simulator::stageMEM() {
    memwb.valid = exmem.valid;
    if(!memwb.valid)
        return;

    // Person 4's job — read exmem fields
    // access memory if needed and fill in memwb fields
}

void Simulator::stageWB() {
    if(!memwb.valid)    // stageWB runs before stageMEM so this works. if this ever changes, it will not work.
        return;

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
