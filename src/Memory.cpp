#include "Memory.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Memory::Memory() {}

int Memory::loadWord(int address) const {
    if (address % 4 != 0) {
        throw std::runtime_error("Unaligned memory access at address: "
                                  + std::to_string(address));
    }
    auto it = mem.find(address);
    if (it == mem.end()) return 0;
    return it->second;
}

void Memory::storeWord(int address, int value) {
    if (address % 4 != 0) {
        throw std::runtime_error("Unaligned memory access at address: "
                                  + std::to_string(address));
    }
    mem[address] = value;
}

void Memory::dump() const {
    std::cout << "\n======================= Memory (non-zero locations) ===============================\n";
    if (mem.empty()) {
        std::cout << "  (empty)\n";
        return;
    }
    for (auto& [addr, val] : mem) {
        std::cout << "  [" << std::setw(6) << addr << "] = " << val << "\n";
    }
    std::cout << "===================================================================================\n";
}