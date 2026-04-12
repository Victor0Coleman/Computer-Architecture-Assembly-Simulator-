//Written by Hanna Apr. 12

#include "Opcode.h"

struct ALUResult {
    int value; //output of the function
    bool zero; //zero flag. if output = 0, this is true.
};

class ALU {
public:
    //op : the operation
    //a : first input
    //b : second input
    ALUResult execute(Opcode op, int a, int b);
};
