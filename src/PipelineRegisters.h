#ifndef PIPELINEREGISTERS_H
#define PIPELINEREGISTERS_H

#include <string>

// ─── IF/ID ───────────────────────────────────────────
// Sits between Fetch and Decode
// IF fetches the instruction and passes it here
struct IF_ID {
    std::string instruction;  // the raw instruction string e.g. "ADD $t0 $t1 $t2"
    int pc;                   // the PC value when this instruction was fetched
    bool valid;               // the difference b/w real instruction and false one (NOP)
    IF_ID() : instruction("NOP"), pc(0), valid(false) {}

    void clear() {*this = IF_ID(); //replaces curr object with default IF_ID
    }
};

// ─── ID/EX ───────────────────────────────────────────
// Sits between Decode and Execute
// ID decodes the instruction and passes everything EX needs here
struct ID_EX {
    std::string type;   // instruction type (R, I, J, and "NOP")
    std::string instruction;  // the raw instruction string e.g. "ADD $t0 $t1 $t2"

    int pc;             // PC value when this instruction was fetched
    // register values read during decode
    int readData1;      // value of $rs
    int readData2;      // value of $rt

    int immediate;      // sign-extended immediate value
    int shamt;          // shift amount for shift instructions
    int jumpOffset;     // jump offset instruction if needed

    // register numbers (needed later for writeback)
    int rs;             // source register number
    int rt;             // source/dest register number
    int rd;             // destination register number (R-type)

    std::string opcode; // e.g. "ADD", "LW", "BEQ"
    std::string funct;  // e.g. For separating opcode and funct

    // control signals
    bool regDst;        // 1 = write to rd, 0 = write to rt
    bool aluSrc;        // 1 = use immediate, 0 = use readData2
    bool memRead;       // 1 = this instruction reads memory (LW)
    bool memWrite;      // 1 = this instruction writes memory (SW)
    bool regWrite;      // 1 = this instruction writes to a register
    bool memToReg;      // 1 = writeback comes from memory, 0 = from ALU
    bool valid;

    ID_EX() : readData1(0), readData2(0), immediate(0),
              rs(0), rt(0), rd(0),instruction("NOP"), type("NOP"), pc(0), opcode("NOP"),
              regDst(false), aluSrc(false), memRead(false),
              memWrite(false), regWrite(false), memToReg(false) {}
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