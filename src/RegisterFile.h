#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <array>
#include <string>

class RegisterFile {
public:
    RegisterFile();

    int read(int regNum) const;
    void write(int regNum, int value);

    void dump() const;

private:
    std::array<int, 32> regs;
};

#endif