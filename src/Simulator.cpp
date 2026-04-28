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
            std::cout << "\n========== CYCLE " << cycle++ << " ==========\n";
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
    int wordPC = pc >> 2;
    if (wordPC < (int)program.size()) {
        ifid.instruction = program[wordPC];
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
    idex.pc = ifid.pc;

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
    exmem = EX_MEM();

    if (idex.isEmpty()) {
        return;
    }

    exmem.valid = true;

    //grab b input and run
    int aluA = idex.readData1;
    int aluB;
    if (idex.ALUOp == Opcode::SLL || idex.ALUOp == Opcode::SRL)
        aluB = idex.shamt;
    else
        aluB = idex.aluSrc ? idex.immediate : idex.readData2;
    
    ALUResult aluOut = alu.execute(idex.ALUOp, aluA, aluB);

    //store outputs
    exmem.aluResult = aluOut.value;
    exmem.zero      = aluOut.zero;
    exmem.writeData = idex.readData2;

    //destination register
    exmem.destReg = idex.regDst ? idex.rd : idex.rt;

    //pass control signals forward
    exmem.memRead  = idex.memRead;
    exmem.memWrite = idex.memWrite;
    exmem.regWrite = idex.regWrite;
    exmem.memToReg = idex.memToReg;

    //beq handling
    if (idex.branch && aluOut.zero) {
        //updates pc to branch target
        pc = idex.pc + 1 + idex.immediate;

        //flush
        ifid.clear();
        idex.clear();

        if (debug) {
            std::cout << "[EX] BEQ taken -> PC = " << pc << "\n";
        }
    }
}

void Simulator::stageMEM(){
    memwb.valid = exmem.valid;
    if(!memwb.valid){ 
        memwb.clear(); //clear if empty or not valid
        return;
    }
    //Memory interactions passes
        memwb.aluResult = exmem.aluResult;  // Pass ALU result for lw (address)
        memwb.regWrite = exmem.regWrite;    // Pass register write signal
        memwb.memToReg = exmem.memToReg;    // Pass memory-to-register signal
        memwb.destReg = exmem.destReg;      // Pass destination register

    //MemOps
    memwb.readData = 0; // default value for non-memory instructions
    if(exmem.memRead){//LW
        memwb.readData = memory.loadWord(exmem.aluResult);}
    if(exmem.memWrite){//SW
        memory.storeWord(exmem.aluResult, exmem.writeData);}
    // Person 4's job — read exmem fields
    // access memory if needed and fill in memwb fields
}

void Simulator::stageWB() {
    if(!memwb.valid)    // stageWB runs before stageMEM so this works. if this ever changes, it will not work.
        return;
    if(memwb.regWrite){
        regFile.write(memwb.destReg, memwb.readData ? memwb.readData : memwb.aluResult);
    }

    // Person 4's job — read memwb fields
    // write result back to register file
}

void Simulator::printState() const {
    std::cout << "\n--- Pipeline Register State ---\n";
    std::cout << "IF/ID:  " << std::hex << ifid.instruction << std::dec << " (PC=" << ifid.pc << ")\n";
    std::cout << "ID/EX:  " << " op=" << idex.opcode
              << " rs=" << registerMap_reverse.at(idex.rs)
              << " rt=" << registerMap_reverse.at(idex.rt)
              << " rd=" << registerMap_reverse.at(idex.rd)
              << " imm=" << idex.immediate
              << " jump=" << idex.jump_offset
              << "\n";

    std::cout << "EX/MEM: aluResult=" << exmem.aluResult
              << " destReg=" << registerMap_reverse.at(exmem.destReg)
              << " writeData=" << exmem.writeData
              << "\n";

    std::cout << "MEM/WB: aluResult=" << memwb.aluResult
            << " destReg=" << registerMap_reverse.at(memwb.destReg)
            << " readData=" << memwb.readData
            << "\n";

    regFile.dump();
    memory.dump();
}
