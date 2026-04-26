#include "controlUnit.h"

void ControlUnit::setsignals(ID_EX& idex) {
    // Implementation of the function

 //reset everything since compulsory miss upon start.
    idex.regDst = false;
    idex.aluSrc = false;
    idex.memRead = false;
    idex.memWrite = false;
    idex.regWrite = false;
    idex.memToReg = false;

    uint32_t op = idex.opcode;

//NOP
    if(idex.type == idex.NOP){
        // Do nothing.
    }


    //R-type 
    else if(idex.type == idex.R){
        idex.regDst = true;
        idex.regWrite = true;
    }

    //J-type
    else if(idex.type == idex.J){
        // This may need a signal. One doesn't exist yet.
    }

    else if(idex.type == idex.I){

        if(op == opcodeMap.at("addi")){
            idex.aluSrc = true;
            idex.regWrite = true;
        }
        //SW
        else if(op == opcodeMap.at("sw")){
            idex.aluSrc = true;
            idex.memWrite = true;
        }
        //LW
        else if(op == opcodeMap.at("lw")){
            idex.aluSrc = true;
            idex.memRead = true;
            idex.regWrite = true;
            idex.memToReg = true;
        }
        //BEQ
        else if(op == opcodeMap.at("beq")){
            // This may need a signal. One doesn't exist yet.
        }

    }

}