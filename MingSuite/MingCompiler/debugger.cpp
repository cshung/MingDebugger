#include "debugger.h"

class debugger_impl
{
public:
    debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface);
    void resume();
private:
    virtual_machine_debugging_interface* m_virtual_machine_debugging_interface;
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

debugger_impl::debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface) : m_virtual_machine_debugging_interface(virtual_machine_debugging_interface)
{
}

void debugger_impl::resume()
{
    this->m_virtual_machine_debugging_interface->resume();
}