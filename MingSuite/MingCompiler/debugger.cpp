#include "debugger.h"
#include <unordered_map>
using namespace std;

class debugger_impl : public debugger_virtual_machine_interface
{
public:
    debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    void resume();
    breakpoint* create_address_breakpoint(int address);
    context get_context();

    virtual void on_breakpoint(int address);
private:
    virtual_machine_debugging_interface* m_virtual_machine_debugging_interface;
    unordered_map<int, breakpoint_impl*> breakpoints;
    breakpoint_impl* breakpoint_to_restore;
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

context debugger::get_context()
{
    return this->impl->get_context();
}

debugger_impl::debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface) : m_virtual_machine_debugging_interface(virtual_machine_debugging_interface)
{
    this->breakpoint_to_restore = nullptr;
}

void debugger_impl::resume()
{
    this->m_virtual_machine_debugging_interface->resume();
}

breakpoint* debugger_impl::create_address_breakpoint(int address)
{
    instruction* break_op = new break_instruction();
    breakpoint_impl* impl = new breakpoint_impl();
    impl->address = address;
    impl->set_debugger(this);
    impl->set_original_instruction(this->m_virtual_machine_debugging_interface->get_instruction(address));
    this->m_virtual_machine_debugging_interface->set_instruction(address, break_op);
    this->breakpoints.insert(make_pair(address, impl));
    return new breakpoint(impl);
}

context debugger_impl::get_context()
{
    return this->m_virtual_machine_debugging_interface->get_context();
}


void debugger_impl::on_breakpoint(int address)
{
    if (this->breakpoint_to_restore != nullptr)
    {
        int breakpoint_address = this->breakpoint_to_restore->address;
        instruction* break_op = this->breakpoint_to_restore->m_original_instruction;
        instruction* original_instruction = this->m_virtual_machine_debugging_interface->get_instruction(breakpoint_address);
        this->breakpoint_to_restore->m_original_instruction = original_instruction;
        this->m_virtual_machine_debugging_interface->set_instruction(breakpoint_address, break_op);
        this->breakpoint_to_restore = nullptr;
    }

    auto probe = this->breakpoints.find(address);
    if (probe != this->breakpoints.end())
    {
        breakpoint_impl* breakpoint = probe->second;
        instruction* original_instruction = breakpoint->m_original_instruction;
        instruction* break_instruction = this->m_virtual_machine_debugging_interface->get_instruction(address);
        breakpoint->m_original_instruction = break_instruction;
        this->m_virtual_machine_debugging_interface->set_instruction(address, original_instruction);
        this->m_virtual_machine_debugging_interface->set_single_step(true);
        this->breakpoint_to_restore = probe->second;
    }
    else
    {
        this->m_virtual_machine_debugging_interface->set_single_step(false);
        this->resume();
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
