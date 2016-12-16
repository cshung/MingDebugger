#pragma once

#include "instructions.h"

class debugger;
class breakpoint;
class virtual_machine_debugging_interface;
class debugger_impl;
class breakpoint_impl;

class debugger
{
public:
    debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    ~debugger();
    void resume();
    breakpoint* create_address_breakpoint(int address);
    void on_breakpoint(int address);
private:
    debugger_impl* impl;
};

class virtual_machine_debugging_interface
{
public:
    virtual void resume() = 0;
    virtual instruction* get_instruction(int address) = 0;
    virtual void set_instruction(int address, instruction* instruction) = 0;
};

class breakpoint
{
public:
    breakpoint();
    ~breakpoint();
    void remove();
private:
    breakpoint_impl* impl;
    friend class debugger_impl;
};
