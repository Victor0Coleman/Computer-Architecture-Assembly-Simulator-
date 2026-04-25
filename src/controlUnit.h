#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H
#include <string>
#include "PipelineRegisters.h"

//looks at instruction and turns op flow ON/OFF the control signals
class ControlUnit{
    static void setsignals(uint32_t op, ID_EX& reg);
};

#endif // CONTROL_UNIT_H




