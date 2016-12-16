#pragma once

#include "instructions.h"

class virtual_machine_impl;

class virtual_machine
{
public:
    virtual_machine();
    ~virtual_machine();

    void run(instruction_sequence instructions, int entry_point);
private:
    virtual_machine_impl* impl;
};
