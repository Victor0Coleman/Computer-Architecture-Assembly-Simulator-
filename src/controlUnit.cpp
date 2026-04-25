#include "controlUnit.h"

void ControlUnit::setsignals(uint32_t op, ID_EX& idex) {
    // Implementation of the function

 //reset everything since compulsory miss upon start.
    idex.regDst = false;
    idex.aluSrc = false;
    idex.memRead = false;
    idex.memWrite = false;
    idex.regWrite = false;
    idex.memToReg = false;

//NOP
    if(idex.type == idex.NOP){
        // What to do with NOP.
    }


//R-type 
    else if(idex.opcode == idex.R){
        idex.regDst = true;
        idex.regWrite = true;
    }
//J-type
    else if(idex.opcode == idex.J){
        //nothing happens
    }

    else if(idex.opcode == idex.I){

        if(idex.op == "addi"){
            idex.aluSrc = true;
            idex.regWrite = true;
        }
        //SW
        else if(op == "sw"){
            idex.aluSrc = true;
            idex.regWrite = true;
        }
        //LW
        else if(op == "lw"){
            idex.aluSrc = true;
            idex.memRead = true;
            idex.regWrite = true;
            idex.memToReg = true;
        }

    }

}