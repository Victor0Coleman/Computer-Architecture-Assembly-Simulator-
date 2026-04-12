/* 
    OPCODES USED IN THE ALU IMPLEMENTATION.
    Can change as needed?
    
    File from Hanna.
*/

//list of the operation names in the assignment. 
enum class Opcode {
    ADD, ADDI, SUB, MUL,
    AND, OR,
    SLL, SRL,
    LW, SW, BEQ, J, NOP
};
