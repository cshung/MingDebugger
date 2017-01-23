#pragma once

#include "instructions.h"
#include "symbols.h"
#include "debugger.h"

class virtual_machine_impl;

class virtual_machine
{
public:
    virtual_machine();
    ~virtual_machine();

    void run(instruction_sequence instructions, int entry_point);
    debugger* debug(instruction_sequence instructions, struct symbols* symbols, int entry_point);
private:
    virtual_machine_impl* impl;
};
