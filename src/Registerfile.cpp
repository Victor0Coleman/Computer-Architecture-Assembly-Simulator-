#include "RegisterFile.h"
#include <iostream>
#include <iomanip>

static const std::string regNames[32] = {
    "$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
    "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
    "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
    "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"
};

RegisterFile::RegisterFile(){
    reset();
}

RegisterFile::RegisterFile() {
    regs.fill(0);
}

int32_t RegisterFile::read(int regNum) const {
    if (regNum < 0 || regNum >= 32){
        std::cerr << "RegisterFile:: read - Invalid register number: " << regNum << std::endl;
        return 0;
    }
    if(regNum == 0) return 0; // $Zero register stays zero
    return regs[regNum];
}

void RegisterFile::write(int regNum, int value) {
        if (regNum < 0 || regNum >= 32){
        std::cerr << "RegisterFile:: write - Invalid register number: " << regNum << std::endl;
        return;
    }
    
    if (regNum == 0) return;    //$zero read only
    regs[regNum] = value;
}

void RegisterFile::dump() const {
    std::cout << "\n=== Register File ===\n";
    for (int i = 0; i < 32; i++) {
        std::cout << std::setw(6) << regNames[i] << ": "
                  << std::setw(10) << regs[i];
        if ((i + 1) % 4 == 0) std::cout << "\n";
        else {std:: cout << " "; 
        }
    }
}