#include "debugger.h"
#include <unordered_map>
#include <iostream>
using namespace std;

class debugger_impl : public debugger_virtual_machine_interface
{
public:
    debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface, symbols* symbols);
    void resume();
    breakpoint* create_address_breakpoint(int address);
    void step_instruction();
    int read_memory(int address);
    void write_memory(int address, int content);
    context get_context();
    void set_context(context c);
    void remove_breakpoint(int address, instruction* original_instruction);
    void stack_walk();
    void step_into();

    // debugger_virtual_machine_interface
    virtual void on_break_instruction();
    virtual void on_single_step();
    virtual void on_terminate();
private:
    virtual_machine_debugging_interface* m_virtual_machine_debugging_interface;
    symbols* m_symbols;
    unordered_map<int, breakpoint_impl*> breakpoints;
    breakpoint_impl* breakpoint_to_restore;
    bool is_single_step_requested;
    bool is_step_into_requested;
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

debugger::debugger(virtual_machine_debugging_interface* virtual_machine_debugging_interface, symbols* symbols)
{
    this->impl = new debugger_impl(virtual_machine_debugging_interface, symbols);
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

void debugger::set_context(context c)
{
    this->impl->set_context(c);
}

void debugger::step_instruction()
{
    this->impl->step_instruction();
}

int debugger::read_memory(int address)
{
    return this->impl->read_memory(address);
}

void debugger::write_memory(int address, int content)
{
    this->impl->write_memory(address, content);
}

void debugger::stack_walk()
{
    this->impl->stack_walk();
}

void debugger::step_into()
{
    this->impl->step_into();
}

debugger_impl::debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface, symbols* symbols) : m_virtual_machine_debugging_interface(virtual_machine_debugging_interface), m_symbols(symbols)
{
    this->breakpoint_to_restore = nullptr;
    this->is_single_step_requested = false;
    this->is_step_into_requested = false;
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

void debugger_impl::set_context(context c)
{
    this->m_virtual_machine_debugging_interface->set_context(c);
}

void debugger_impl::step_instruction()
{
    this->m_virtual_machine_debugging_interface->set_single_step(true);
    this->is_single_step_requested = true;
    this->resume();
}

int debugger_impl::read_memory(int address)
{
    return this->m_virtual_machine_debugging_interface->read_memory(address);
}

void debugger_impl::write_memory(int address, int content)
{
    this->m_virtual_machine_debugging_interface->write_memory(address, content);
}

void debugger_impl::on_break_instruction()
{
    this->is_single_step_requested = false;
    this->is_step_into_requested = false;

    int break_instruction_address = this->m_virtual_machine_debugging_interface->get_context().ip - 1;
    breakpoint_impl* breakpoint = this->breakpoints[break_instruction_address];
    instruction* original_instruction = breakpoint->m_original_instruction;
    instruction* break_instruction = this->m_virtual_machine_debugging_interface->get_instruction(break_instruction_address);
    delete break_instruction;
    this->m_virtual_machine_debugging_interface->set_instruction(break_instruction_address, original_instruction);
    this->m_virtual_machine_debugging_interface->set_single_step(true);
    context c = this->m_virtual_machine_debugging_interface->get_context();
    c.ip--;
    this->m_virtual_machine_debugging_interface->set_context(c);
    this->breakpoint_to_restore = breakpoint;

}
void debugger_impl::on_single_step()
{
    if (this->is_step_into_requested)
    {
        int ip = this->m_virtual_machine_debugging_interface->get_context().ip;
        bool reached_next_statement = false;
        for (auto&& statement : this->m_symbols->statements)
        {
            if (ip == statement.start_address)
            {
                reached_next_statement = true;
            }
        }
        if (!reached_next_statement)
        {
            this->m_virtual_machine_debugging_interface->set_single_step(true);
            this->m_virtual_machine_debugging_interface->resume();
        }
        else
        {
            this->is_step_into_requested = false;
        }
    }
    else
    {
        if (this->breakpoint_to_restore != nullptr)
        {
            int breakpoint_address = this->breakpoint_to_restore->address;
            instruction* original_instruction = this->m_virtual_machine_debugging_interface->get_instruction(breakpoint_address);
            this->breakpoint_to_restore->m_original_instruction = original_instruction;
            this->m_virtual_machine_debugging_interface->set_instruction(breakpoint_address, new break_instruction());
            this->breakpoint_to_restore = nullptr;
        }
        if (is_single_step_requested)
        {
            this->is_single_step_requested = false;
        }
        else
        {
            this->m_virtual_machine_debugging_interface->set_single_step(false);
            this->m_virtual_machine_debugging_interface->resume();
        }
    }
}

void debugger_impl::on_terminate()
{
    cout << "Process terminated" << endl;
}
void debugger_impl::remove_breakpoint(int address, instruction* original_instruction)
{
    this->m_virtual_machine_debugging_interface->set_instruction(address, original_instruction);

    // If the currently removing breakpoint is to be restored - do not do that anymore.
    if (this->breakpoint_to_restore != nullptr && this->breakpoint_to_restore->address == address)
    {
        this->breakpoint_to_restore = nullptr;
        this->m_virtual_machine_debugging_interface->set_single_step(false);
    }
}

void debugger_impl::stack_walk()
{
    context c = this->get_context();
    int ip = c.ip;
    int sp = c.sp;
    while (ip != -1)
    {
        for (auto&& function_symbol : this->m_symbols->functions)
        {
            if (function_symbol.entry_point <= ip && ip < function_symbol.after_exit)
            {
                cout << function_symbol.function_name;

                for (auto&& local_symbols : function_symbol.locals)
                {
                    cout << " ";
                    cout << local_symbols.local_name;
                    cout << " = ";
                    cout << this->m_virtual_machine_debugging_interface->read_memory(sp + local_symbols.address);
                    cout << " ";
                }
                cout << endl;

                // Unwinding the push instruction
                push_instruction* push = (push_instruction*)this->m_virtual_machine_debugging_interface->get_instruction(function_symbol.entry_point);
                sp += push->offset;

                // Unwinding the call instruction
                ip = this->m_virtual_machine_debugging_interface->read_memory(++sp);
            }
        }
    }
}

void debugger_impl::step_into()
{
    this->is_step_into_requested = true;
    this->m_virtual_machine_debugging_interface->set_single_step(true);
    this->resume();
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
    this->m_debugger->remove_breakpoint(address, this->m_original_instruction);
}

void breakpoint_impl::set_debugger(debugger_impl* debugger)
{
    this->m_debugger = debugger;
}
void breakpoint_impl::set_original_instruction(instruction* original_instruction)
{
    this->m_original_instruction = original_instruction;
}
