#include "controlUnit.h"

void ControlUnit::setsignals(const std::string& op, ID_EX& reg) {
    // Implementation of the function

 //reset everything since compulsory miss upon start.
    idex.regDst = false;
    idex.aluSrc = false;
    idex.memRead = false;
    idex.memWrite = false;
    idex.regWrite = false;
    idex.memToReg = false;

//R-type 
    if(op == "add"||op == "or" || op == "and"){
        reg.regDst = true;
        reg.regWrite = true;
    }
//J-type
    else if(op == "j"){
        //nothing happens
    }
    else if(op == "addi"){
        reg.aluSrc = true;
        reg.regWrite = true;
    }
//SW
    else if(op == "sw"){
        reg.aluSrc = true;
        reg.regWrite = true;
    }
//LW
    else if(op == "lw"){
        reg.aluSrc = true;
        reg.memRead = true;
        reg.regWrite = true;
        reg.memToReg = true;
    }

    }
//SW
    else if(){

    }


}