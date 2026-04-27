#include "ALU.h"
#include <stdexcept>

//using this returns the output and zero flag
ALUResult ALU::execute(Opcode op, int a, int b) {
    //declares initial result to return
    ALUResult res;
    res.value = 0;
    res.zero = false;

    switch (op) {
        //currently not built to handle immediates on its own - can change
        case Opcode::ADD:
        case Opcode::ADDI:
            res.value = a + b;
            break;

        case Opcode::SUB:
            res.value = a - b;
            break;

        case Opcode::MUL:
            res.value = a * b;
            break;

        case Opcode::AND:
            res.value = a & b;
            break;

        case Opcode::OR:
            res.value = a | b;
            break;

        case Opcode::SLL:
            res.value = a << b;
            break;

        case Opcode::SRL:
            res.value = static_cast<unsigned int>(a) >> b;
            break;


        case Opcode::BEQ:
            res.value = a - b;
            break;

        case Opcode::J:
            // do nothing but add this line so it runs
            // maybe pretend a nop got passed forward

        case Opcode::NOP:
            res.value = 0;
            break;

        default:
            throw std::invalid_argument("Unsupported ALU operation");
    }

    res.zero = (res.value == 0);
    return res;
}