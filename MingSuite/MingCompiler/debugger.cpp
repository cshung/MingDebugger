#include "debugger.h"
#include <unordered_map>
using namespace std;

class debugger_impl
{
public:
    debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    void resume();
    breakpoint* create_address_breakpoint(int address);
private:
    virtual_machine_debugging_interface* m_virtual_machine_debugging_interface;
    unordered_map<int, breakpoint*> breakpoints;
};

class breakpoint_impl
{
public:
    void remove();
    void set_debugger(debugger_impl* debugger);
    void set_original_instruction(instruction* original_instruction);
    int address;
    debugger_impl* m_debugger;
    instruction* m_original_instruction;
};

debugger::debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface)
{
    this->impl = new debugger_impl(virtual_machine_debugging_interface);
}

debugger::~debugger()
{
    delete this->impl;
}

void debugger::resume()
{
    this->impl->resume();
}

breakpoint* debugger::create_address_breakpoint(int address)
{
    return this->impl->create_address_breakpoint(address);
}

debugger_impl::debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface) : m_virtual_machine_debugging_interface(virtual_machine_debugging_interface)
{
}

void debugger_impl::resume()
{
    this->m_virtual_machine_debugging_interface->resume();
}

breakpoint* debugger_impl::create_address_breakpoint(int address)
{
    breakpoint* result = new breakpoint();
    break_instruction* break_op = new break_instruction();
    result->impl->set_debugger(this);
    result->impl->set_original_instruction(this->m_virtual_machine_debugging_interface->get_instruction(address));
    this->m_virtual_machine_debugging_interface->set_instruction(address, break_op);
    this->breakpoints.insert(make_pair(address, result));
    return result;
}

void debugger::on_breakpoint(int address)
{
    // TODO: So we reached a breakpoint, now we need to be able to resume without losing it!
}

breakpoint::breakpoint()
{
    this->impl = new breakpoint_impl();
}

breakpoint::~breakpoint()
{
    delete this->impl;
}

void breakpoint::remove()
{
    this->impl->remove();
}

void breakpoint_impl::remove()
{
    // do something
}

void breakpoint_impl::set_debugger(debugger_impl* debugger)
{
    this->m_debugger = debugger;
}
void breakpoint_impl::set_original_instruction(instruction* original_instruction)
{
    this->m_original_instruction = original_instruction;
}
