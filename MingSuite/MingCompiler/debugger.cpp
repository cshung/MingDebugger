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
    void step_over();
    source_span get_source_span();

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
    bool is_step_over_requested;
    breakpoint* m_step_over_breakpoint;
    int m_step_over_frame_sp;
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

void debugger::step_over()
{
    this->impl->step_over();
}

source_span debugger::get_source_span()
{
    return this->impl->get_source_span();
}

debugger_impl::debugger_impl(virtual_machine_debugging_interface* virtual_machine_debugging_interface, symbols* symbols) : m_virtual_machine_debugging_interface(virtual_machine_debugging_interface), m_symbols(symbols)
{
    this->breakpoint_to_restore = nullptr;
    this->is_single_step_requested = false;
    this->is_step_into_requested = false;
    this->is_step_over_requested = false;
    this->m_step_over_breakpoint = nullptr;
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
    context c = this->m_virtual_machine_debugging_interface->get_context();
    c.ip--;
    this->m_virtual_machine_debugging_interface->set_context(c);

    if (this->m_step_over_breakpoint != nullptr && break_instruction_address == this->m_step_over_breakpoint->get_address())
    {
        if (this->get_context().sp == this->m_step_over_frame_sp)
        {
            this->m_step_over_breakpoint->remove();
            this->m_step_over_breakpoint = nullptr;
            this->m_virtual_machine_debugging_interface->set_single_step(true);
            this->resume();
        }
        else
        {
            this->m_virtual_machine_debugging_interface->set_single_step(true);
            this->breakpoint_to_restore = breakpoint;
            this->resume();
        }
    }
    else
    {
        this->is_step_over_requested = false;
        this->m_virtual_machine_debugging_interface->set_single_step(true);
        this->breakpoint_to_restore = breakpoint;
    }

}
void debugger_impl::on_single_step()
{
    if (this->breakpoint_to_restore != nullptr)
    {
        int breakpoint_address = this->breakpoint_to_restore->address;
        instruction* original_instruction = this->m_virtual_machine_debugging_interface->get_instruction(breakpoint_address);
        this->breakpoint_to_restore->m_original_instruction = original_instruction;
        this->m_virtual_machine_debugging_interface->set_instruction(breakpoint_address, new break_instruction());
        this->breakpoint_to_restore = nullptr;
    }
    if (this->is_step_into_requested || this->is_step_over_requested)
    {
        if (this->m_step_over_breakpoint != nullptr)
        {
            this->m_virtual_machine_debugging_interface->set_single_step(false);
            this->resume();
        }
        else
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
                if (this->is_step_over_requested)
                {
                    instruction* current_instruction = this->m_virtual_machine_debugging_interface->get_instruction(ip);
                    if (current_instruction->get_instruction_type() == call_instruction_type)
                    {
                        this->m_virtual_machine_debugging_interface->set_single_step(false);
                        this->m_step_over_breakpoint = this->create_address_breakpoint(ip + 1);
                        this->m_step_over_frame_sp = this->m_virtual_machine_debugging_interface->get_context().sp;
                        this->m_virtual_machine_debugging_interface->resume();
                    }
                    else
                    {
                        this->m_virtual_machine_debugging_interface->set_single_step(true);
                        this->m_virtual_machine_debugging_interface->resume();
                    }
                }
                else
                {
                    this->m_virtual_machine_debugging_interface->set_single_step(true);
                    this->m_virtual_machine_debugging_interface->resume();
                }
            }
            else
            {
                this->is_step_into_requested = false;
                this->is_step_over_requested = false;
                this->m_step_over_breakpoint = nullptr;
            }
        }
    }
    else
    {        
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

void debugger_impl::step_over()
{
    this->is_step_over_requested = true;
    this->m_virtual_machine_debugging_interface->set_single_step(true);
    this->resume();
}

source_span debugger_impl::get_source_span()
{
    int address = this->get_context().ip;

    // Find the statement with smallest address while at least as large as the current IP
    int best_address = -1;
    source_span best_source_span;
    for (auto&& statement : this->m_symbols->statements)
    {
        if (statement.start_address >= address)
        {
            if (best_address == -1)
            {
                best_address = statement.start_address;
                best_source_span = statement.source_span;
            }
            else if (statement.start_address < best_address)
            {
                best_address = statement.start_address;
                best_source_span = statement.source_span;
            }
        }
    }

    return best_source_span;
}

breakpoint::breakpoint(breakpoint_impl* impl)
{
    this->impl = impl;
}

breakpoint::~breakpoint()
{
    delete this->impl;
}

int breakpoint::get_address()
{
    return this->impl->address;
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
