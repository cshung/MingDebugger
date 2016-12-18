#pragma once

#include "instructions.h"

class debugger;
class breakpoint;
class virtual_machine_debugging_interface;
class debugger_virtual_machine_interface;
class debugger_impl;
class breakpoint_impl;

struct context
{
    int r1;
    int r2;
    int r3;
    int r4;
    int sp;
    int ip;
};

class debugger
{
public:
    debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    ~debugger();

    void resume();
    breakpoint* create_address_breakpoint(int address);
    virtual context get_context();
    virtual void set_context(context c);
    virtual void step_instruction();
    virtual int read_memory(int address);
    virtual void write_memory(int address, int content);

    debugger_virtual_machine_interface* get_debugger_virtual_machine_interface();
private:
    debugger_impl* impl;
};

class virtual_machine_debugging_interface
{
public:
    virtual void resume() = 0;
    virtual instruction* get_instruction(int address) = 0;
    virtual void set_instruction(int address, instruction* instruction) = 0;
    virtual void set_single_step(bool on) = 0;
    virtual context get_context() = 0;
    virtual void set_context(context c) = 0;
    virtual int read_memory(int address) = 0;
    virtual void write_memory(int address, int content) = 0;
};

class debugger_virtual_machine_interface
{
public:
    virtual void on_break_instruction() = 0;
    virtual void on_single_step() = 0;
    virtual void on_terminate() = 0;
};

class breakpoint
{
public:
    breakpoint(breakpoint_impl* impl);
    ~breakpoint();
    void remove();
private:
    breakpoint_impl* impl;
};
