#include "debugger.h"
#include <unordered_map>
using namespace std;

class debugger_impl : public debugger_virtual_machine_interface
{
public:
    debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    void resume();
    breakpoint* create_address_breakpoint(int address);
    virtual void on_breakpoint(int address);
private:
    virtual_machine_debugging_interface* m_virtual_machine_debugging_interface;
    unordered_map<int, breakpoint_impl*> breakpoints;
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

debugger_virtual_machine_interface* debugger::get_debugger_virtual_machine_interface()
{
    return this->impl;
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
    break_instruction* break_op = new break_instruction();
    breakpoint_impl* impl = new breakpoint_impl();
    impl->set_debugger(this);
    impl->set_original_instruction(this->m_virtual_machine_debugging_interface->get_instruction(address));
    this->m_virtual_machine_debugging_interface->set_instruction(address, break_op);
    this->breakpoints.insert(make_pair(address, impl));
    return new breakpoint(impl);
}

void debugger_impl::on_breakpoint(int address)
{
    auto probe = this->breakpoints.find(address);
    if (probe != this->breakpoints.end())
    {
        instruction* original_instruction = probe->second->m_original_instruction;
        this->m_virtual_machine_debugging_interface->set_instruction(address, original_instruction);
        // Now the breakpoint is gone, we need to re-enable it
        // Entering single stepping
    }
}

breakpoint::breakpoint(breakpoint_impl* impl)
{
    this->impl = impl;
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
