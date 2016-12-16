#pragma once

class debugger_impl;

class virtual_machine_debugging_interface
{
public:
    virtual void resume() = 0;
};

class debugger
{
public:
    debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    ~debugger();
    void resume();
    void break_on_address(int address);
private:
    debugger_impl* impl;
};
