#include "encoder.h"
#include "Mappings.h"
#include <iostream>

// Input: a list which contains that line of assembly code (i.e. "add", "$t1", "$t2", "$t3")
// Output: A machine instruction
uint32_t encode(const std::vector<std::string>& tokens,
                const std::unordered_map<std::string, int>& labelTable,
                int pc){
    uint32_t machineInstruction = 0; 
    uint8_t opcode = 0;
    std::string op = tokens[0];

    // Really, NOP -> Nothing more to do.   Technically this line is unnecessary.
    if(op == "NOP"){
        return 0;
    }

    int type =
        functMap.find(op) != functMap.end() ? 0
        : op == "j" ? 2
        : opcodeMap.find(op) != opcodeMap.end() ? 1
        : -1; 


    machineInstruction = _split_by_type(tokens, labelTable, type, pc);

    return machineInstruction;

}

uint32_t encode(const std::string& str, 
                const std::unordered_map<std::string, int>& labelTable,
                int pc){
    return encode(_tokenize(str), labelTable, pc);
}


uint32_t _split_by_type(const std::vector<std::string>& tokens,
                        const std::unordered_map<std::string, int>& labelTable, 
                        int type, int pc){
    switch(type){
        case 0: return _encode_R(tokens);
        case 1: return _encode_I(tokens, labelTable, pc);
        case 2: return _encode_J(tokens, labelTable);
        default:return 0; // NOP
    }
}

uint32_t _encode_R(const std::vector<std::string>& tokens){
    uint32_t machineInstruction = 0;
    std::string op = tokens[0];

    if(tokens.size() != 4){
        std::cerr << "R-type instruction without 4 params";
        return 0;
    }

    uint8_t rd = 0, rs = 0, rt = 0, shamt = 0, funct = 0;

    funct = functMap.at(tokens[0]);
    rd = registerMap.at(tokens[1]);

    if(op == "sll" || op == "srl"){
        shamt = std::stoi(tokens[3]);    // may lead to interesting results when input >= 32 
        rt = registerMap.at(tokens[2]);
    }else{
        rs = registerMap.at(tokens[2]);
        rt = registerMap.at(tokens[3]);
    }


    machineInstruction |= rs << 21;
    machineInstruction |= rt << 16;
    machineInstruction |= rd << 11;
    machineInstruction |= shamt << 6;
    machineInstruction |= funct;


    return machineInstruction;

}

uint32_t _encode_I(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable, int pc){
    uint32_t machineInstruction = 0;
    uint8_t opcode = opcodeMap.at(tokens[0]);
    uint8_t rs = 0, rt = 0;
    int16_t immediate = 0;
    
    // EASILY REMOVABLE 4-token check
    if(tokens.size() != 4){
        std::cerr << "I-type instruction without 4 params";
        return 0;
    }

    switch(opcode){
        case 0x04: {  // BEQ $rs $rt ADDR
            rs = registerMap.at(tokens[1]);
            rt = registerMap.at(tokens[2]);

            // if not label, treat as address
            int16_t target = labelTable.count(tokens[3]) ? labelTable.at(tokens[3]) : std::stoi(tokens[3]); 

            immediate = (target - (pc + 4)) >> 2;        
            break;
        }
        case 0x08:  // ADDI $rt $rs IMM
            rt = registerMap.at(tokens[1]);
            rs = registerMap.at(tokens[2]);
            immediate = std::stoi(tokens[3]);   
            break;
        case 0x23:  // LW $rt IMM $rs
            rt = registerMap.at(tokens[1]);
            immediate = std::stoi(tokens[2]);
            rs = registerMap.at(tokens[3]);
            break;
        case 0x2b:  // SW $rt IMM $rs 
            rt = registerMap.at(tokens[1]);
            immediate = std::stoi(tokens[2]);
            rs = registerMap.at(tokens[3]);
            break;
        default:    // NOT In this assignment.      (This line should NOT be reached)
            std::cerr << "No such I-type instruction!";
            return 0; //nop
    }
    
    machineInstruction |= opcode << 26;
    machineInstruction |= rs << 21;
    machineInstruction |= rt << 16;
    machineInstruction |= immediate & 0xFFFF;

    return machineInstruction;
}

uint32_t _encode_J(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable){
    uint32_t machineInstruction = 0;

    if(tokens.size() != 2){
        std::cerr << "J-type instruction without 2 params";
        return 0;
    }

    uint8_t opcode = 0;
    uint32_t jump_offset = 0, address = 0;

    opcode = opcodeMap.at(tokens[0]);
    // if not label, treat as address
    address = labelTable.count(tokens[1]) ? labelTable.at(tokens[1]) : std::stoi(tokens[1]);

    if(address & 3){
        std::cerr << "Unaligned jump address! Expected a multiple of 4";
        return 0;
    }

    // remove highest 4 bits and lowest 2 bits for a 26-bit result
    jump_offset = (address & 0x0FFFFFFC) >> 2; 

    machineInstruction |= opcode << 26;
    machineInstruction |= jump_offset;

    return machineInstruction;


}



// Input: a line of assembly code
// Output: a list which contains that line of assembly code
std::vector<std::string> _tokenize(const std::string& str){
    std::vector<std::string> tokens;
    std::string current;

    for (char c : str) {
        if (c == ' ' || c == ',' || c == '(' || c == ')') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

