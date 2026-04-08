#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <array>
#include <string>
#include <cstdint>

class RegisterFile {
public:
    RegisterFile();

    int32_t read(int regNum) const; //guarentees its 32-bit no matter what
    void write(int regNum, int value);

    void dump() const;
    void reset(); // Resets all registers

private:
    std::array<int, 32> regs;
};

#endif