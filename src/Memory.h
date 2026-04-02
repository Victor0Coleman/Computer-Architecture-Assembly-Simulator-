#ifndef MEMORY_H
#define MEMORY_H

#include <unordered_map>

class Memory {
public:
    Memory();

    int  loadWord(int address) const;
    void storeWord(int address, int value);

    void dump() const;

private:
    std::unordered_map<int, int> mem;
};

#endif