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
        idex.ALUOp = Opcode::NOP;
    }


    //R-type 
    else if(idex.type == idex.R){
        idex.regDst = true;
        idex.regWrite = true;
        
        // Set ALUOp
        if(idex.funct == functMap.at("add"))
            idex.ALUOp = Opcode::ADD;
        else if(idex.funct == functMap.at("sub"))
            idex.ALUOp = Opcode::SUB;
        else if(idex.funct == functMap.at("mul"))
            idex.ALUOp = Opcode::MUL;
        else if(idex.funct == functMap.at("and"))
            idex.ALUOp = Opcode::AND;
        else if(idex.funct == functMap.at("or"))
            idex.ALUOp = Opcode::OR;
        else if(idex.funct == functMap.at("sll"))
            idex.ALUOp = Opcode::SLL;
        else if(idex.funct == functMap.at("srl"))
            idex.ALUOp = Opcode::SRL;

    }

    //J-type
    else if(idex.type == idex.J){
        // This may need a signal. One doesn't exist yet.
        idex.ALUOp = Opcode::J;
    }

    else if(idex.type == idex.I){

        if(op == opcodeMap.at("addi")){
            idex.aluSrc = true;
            idex.regWrite = true;
            idex.ALUOp = Opcode::ADDI;
        }
        //SW
        else if(op == opcodeMap.at("sw")){
            idex.aluSrc = true;
            idex.memWrite = true;
            idex.ALUOp = Opcode::SW;
        }
        //LW
        else if(op == opcodeMap.at("lw")){
            idex.aluSrc = true;
            idex.memRead = true;
            idex.regWrite = true;
            idex.memToReg = true;
            idex.ALUOp = Opcode::LW;
        }
        //BEQ
        else if(op == opcodeMap.at("beq")){
            idex.branch = true;
            idex.ALUOp = Opcode::BEQ;
        }

    }



}