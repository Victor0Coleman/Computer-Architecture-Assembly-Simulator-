/* 
    OPCODES USED IN THE ALU IMPLEMENTATION.
    Can change as needed?
    
    File from Hanna.
*/

//list of the operation names in the assignment. 
#ifndef OPCODE_H
#define OPCODE_H

enum class Opcode {
    ADDI,
    MUL,
    SLL,
    SRL,
    ADD,
    SUB,
    AND,
    OR,
    SLT,
    LW,
    SW,
    BEQ,
    J,
    NOP
};

#endif // OPCODE_H
