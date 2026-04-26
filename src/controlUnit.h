#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H
#include <string>
#include "PipelineRegisters.h"
#include "Mappings.h"

//looks at instruction and turns op flow ON/OFF the control signals
class ControlUnit{
public:
    static void setsignals(ID_EX& idex);
};

#endif // CONTROL_UNIT_H




