#pragma once

#include "instructions.h"
#include "symbols.h"

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
    debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface, symbols* symbols);
    ~debugger();

    // Low level debugging
    void resume();
    breakpoint* create_address_breakpoint(int address);
    context get_context();
    void set_context(context c);
    void step_instruction();
    int read_memory(int address);
    void write_memory(int address, int content);

    // Source level debugging
    void stack_walk();
    void step_into();
    void step_over();

    source_span get_source_span();

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
    int get_address();
    void remove();
private:
    breakpoint_impl* impl;
};
