#include "Mappings.h"

    const std::unordered_map<std::string, uint8_t> registerMap = {
        {"$zero", 0},
        {"$at", 1},
        {"$v0", 2},
        {"$v1", 3},
        {"$a0", 4},
        {"$a1", 5},
        {"$a2", 6},
        {"$a3", 7},
        {"$t0", 8},
        {"$t1", 9},
        {"$t2", 10},
        {"$t3", 11},
        {"$t4", 12},
        {"$t5", 13},
        {"$t6", 14},
        {"$t7", 15},
        {"$s0", 16},
        {"$s1", 17},
        {"$s2", 18},
        {"$s3", 19},
        {"$s4", 20},
        {"$s5", 21},
        {"$s6", 22},
        {"$s7", 23},
        {"$t8", 24},
        {"$t9", 25},
        {"$k0", 26},
        {"$k1", 27},
        {"$gp", 28},
        {"$sp", 29},
        {"$fp", 30},
        {"$ra", 31}
    };

    // operations to support with respective MIPS opcode
    // NOTE THAT "R-type" instructions actually have opcode 0, and FUNCTION CODE (some number). For such instructions i am simply (incorrectly) placing their fn code as if it was an opcode
    const std::unordered_map<std::string, uint8_t> functMap = {
        // FUNCTION CODES
        {"add", 0x20},
        {"sub", 0x22},
        {"mul", 0x018}, // NOTE: this is MULT's function code, MUL is a pseudo-instruction in real MIPS 
        {"and", 0x24},
        {"or", 0x25},
        {"sll", 0x00},
        {"srl", 0x02}
    };

    const std::unordered_map<std::string, uint8_t> opcodeMap = {
        // OPCODEs
     /* {any R-format instruction, 0x00}*/// Just a note.   
        {"addi", 0x08},
        {"lw", 0x23},
        {"sw", 0x2b},
        {"beq", 0x04},
        {"j", 0x02},
    };
