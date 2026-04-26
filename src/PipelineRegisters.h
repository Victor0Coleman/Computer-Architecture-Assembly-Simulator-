#ifndef PIPELINEREGISTERS_H
#define PIPELINEREGISTERS_H

#include <string>
#include <cstdint>
#include "Opcode.h"

// ─── IF/ID ───────────────────────────────────────────
// Sits between Fetch and Decode
// IF fetches the instruction and passes it here
struct IF_ID {
    uint32_t instruction;     // the raw instruction as a binary string
    int pc;                   // the PC value when this instruction was fetched

    IF_ID() : instruction(0), pc(0) {}
    void clear() {*this = IF_ID(); //replaces curr object with default IF_ID
    }
};

// ─── ID/EX ───────────────────────────────────────────
// Sits between Decode and Execute
// ID decodes the instruction and passes everything EX needs here
struct ID_EX {
    int pc;             // PC value when this instruction was fetched
    // register values read during decode
    int readData1;      // value of $rs
    int readData2;      // value of $rt

    int immediate;      // sign-extended immediate value
    int jump_offset;     // 26-bit offset for jumps

    enum Type{R, I, J, NOP} type;   

    // register numbers (needed later for writeback)
    int rs;             // source register number
    int rt;             // source/dest register number
    int rd;             // destination register number (R-type)
    int shamt;          // shift amount (R-type)
    int funct;           // function code (R-type)

    int opcode; // This is now a number opcode.

    // control signals
    bool regDst;        // 1 = write to rd, 0 = write to rt
    bool aluSrc;        // 1 = use immediate, 0 = use readData2
    bool memRead;       // 1 = this instruction reads memory (LW)
    bool memWrite;      // 1 = this instruction writes memory (SW)
    bool regWrite;      // 1 = this instruction writes to a register
    bool memToReg;      // 1 = writeback comes from memory, 0 = from ALU
    bool branch;        // 1 = branch instruction
    Opcode ALUOp;       // Not an "Opcode" per-se; merely represent an operation for ALU to perform

    ID_EX() : readData1(0), readData2(0), immediate(0), type(NOP),
              rs(0), rt(0), rd(0), shamt(0), funct(0), opcode(0),
              regDst(false), aluSrc(false), memRead(false), memWrite(false),
              regWrite(false), memToReg(false), branch(false), ALUOp(Opcode::NOP) {}
};

// ─── EX/MEM ──────────────────────────────────────────
// Sits between Execute and Memory
// EX runs the ALU and passes results here
struct EX_MEM {
    int aluResult;      // result of the ALU operation
    int writeData;      // value of $rt (used by SW to store to memory)
    int destReg;        // which register to write back to

    // control signals (passed through from ID/EX)
    bool memRead;
    bool memWrite;
    bool regWrite;
    bool memToReg;

    EX_MEM() : aluResult(0), writeData(0), destReg(0),
               memRead(false), memWrite(false),
               regWrite(false), memToReg(false) {}

               void clear() {*this = EX_MEM();} //replaces curr object with default EX_MEM
};

// ─── MEM/WB ──────────────────────────────────────────
// Sits between Memory and Writeback
// MEM accesses memory and passes results here
struct MEM_WB {
    int aluResult;      // passed through for non-memory instructions
    int readData;       // data read from memory (used by LW)
    int destReg;        // which register to write back to

    // control signals
    bool regWrite;      // 1 = write result back to register file
    bool memToReg;      // 1 = write memory data, 0 = write ALU result

    bool valid;

    MEM_WB() : aluResult(0), readData(0), destReg(0),
               regWrite(false), memToReg(false) {}

    void clear() {*this = MEM_WB();} //replaces curr object with default MEM_WB
};

#endif