#include "virtual_machine.h"
#include <vector>
#include <iostream>
using namespace std;

class virtual_machine_impl : virtual_machine_debugging_interface
{
public:
    virtual_machine_impl();
    void run(instruction_sequence instructions, int entry_point);
    debugger* debug(instruction_sequence instructions, symbols* symbols, int entry_point);

    // virtual_machine_debugging_interface
    virtual void resume();
    virtual instruction* get_instruction(int address);
    virtual void set_instruction(int address, instruction* instruction);
    virtual void set_single_step(bool on);
    virtual context get_context();
    virtual void set_context(context c);
    virtual int read_memory(int address);
    virtual void write_memory(int address, int content);
private:
    void setup(instruction_sequence instructions, int entry_point);
    void execute(instruction* instruction);
    void execute(load_instruction* instruction);
    void execute(load_immediate_instruction* instruction);
    void execute(compare_instruction* instruction);
    void execute(store_instruction* instruction);
    void execute(branch_on_zero_instruction* instruction);
    void execute(branch_instruction* instruction);
    void execute(plus_instruction* instruction);
    void execute(minus_instruction* instruction);
    void execute(call_instruction* instruction);
    void execute(return_instruction* instruction);
    void execute(push_instruction* instruction);
    void execute(pop_instruction* instruction);
    void execute(print_instruction* instruction);
    void execute(break_instruction* instruction);
    vector<instruction*> code_memory;
    vector<int> data_memory;
    int ip;
    int sp;
    int registers[5];

    bool is_single_step;
    bool break_instruction_executed;
    debugger_virtual_machine_interface* m_debugger_virtual_machine_interface;
};

virtual_machine::virtual_machine()
{
    this->impl = new virtual_machine_impl();
}

virtual_machine::~virtual_machine()
{
    delete this->impl;
}

void virtual_machine::run(instruction_sequence instructions, int entry_point)
{
    this->impl->run(instructions, entry_point);
}

debugger* virtual_machine::debug(instruction_sequence instructions, symbols* symbols, int entry_point)
{
    return this->impl->debug(instructions, symbols, entry_point);
}

virtual_machine_impl::virtual_machine_impl()
{
    this->m_debugger_virtual_machine_interface = nullptr;
    this->is_single_step = false;
    this->break_instruction_executed = false;
}

void virtual_machine_impl::setup(instruction_sequence instructions, int entry_point)
{    
    int stack_size = 10000;
    data_memory.resize(stack_size);
    instruction* cursor = instructions.head;
    while (cursor != nullptr)
    {
        this->code_memory.push_back(cursor);
        cursor = cursor->next;
    }

    // Emulating a call coming from ip = -1;
    sp = stack_size - 1;
    data_memory[sp] = -1;
    sp--;
    ip = entry_point;
}

void virtual_machine_impl::run(instruction_sequence instructions, int entry_point)
{
    this->setup(instructions, entry_point);
    this->resume();
}

debugger* virtual_machine_impl::debug(instruction_sequence instructions, symbols* symbols, int entry_point)
{
    this->setup(instructions, entry_point);
    debugger* result = new debugger(this, symbols);
    this->m_debugger_virtual_machine_interface = result->get_debugger_virtual_machine_interface();
    return result;
}

void virtual_machine_impl::resume()
{
    while (this->ip != -1)
    {
        instruction* current = code_memory[this->ip];
        this->execute(current);
        if (this->break_instruction_executed)
        {
            break;
        }
        if (this->is_single_step)
        {
            break;
        }
    }
    if (this->break_instruction_executed)
    {
        this->break_instruction_executed = false;
        this->m_debugger_virtual_machine_interface->on_break_instruction();
    }
    else if (this->is_single_step)
    {
        this->m_debugger_virtual_machine_interface->on_single_step();
    }
    else if (this->ip == -1)
    {
        this->m_debugger_virtual_machine_interface->on_terminate();
    }
}
instruction* virtual_machine_impl::get_instruction(int address)
{
    return this->code_memory[address];
}

void virtual_machine_impl::set_instruction(int address, instruction* instruction)
{
    this->code_memory[address] = instruction;
}

void virtual_machine_impl::set_single_step(bool on)
{
    this->is_single_step = on;
}

context virtual_machine_impl::get_context()
{
    context result;
    result.r1 = this->registers[1];
    result.r2 = this->registers[2];
    result.r3 = this->registers[3];
    result.r4 = this->registers[4];
    result.sp = this->sp;
    result.ip = this->ip;
    return result;
}

void virtual_machine_impl::set_context(context c)
{
    this->registers[1] = c.r1;
    this->registers[2] = c.r2;
    this->registers[3] = c.r3;
    this->registers[4] = c.r4;
    this->sp = c.sp;
    this->ip = c.ip;
}

int virtual_machine_impl::read_memory(int address)
{
    return this->data_memory[address];
}

void virtual_machine_impl::write_memory(int address, int content)
{
    this->data_memory[address] = content;
}

void virtual_machine_impl::execute(instruction* instruction)
{
    switch (instruction->get_instruction_type())
    {
    case instruction_type::load_instruction_type:
        execute((load_instruction*)instruction);
        break;
    case instruction_type::load_immediate_instruction_type:
        execute((load_immediate_instruction*)instruction);
        break;
    case instruction_type::compare_instruction_type:
        execute((compare_instruction*)instruction);
        break;
    case instruction_type::store_instruction_type:
        execute((store_instruction*)instruction);
        break;
    case instruction_type::branch_on_zero_instruction_type:
        execute((branch_on_zero_instruction*)instruction);
        break;
    case instruction_type::branch_instruction_type:
        execute((branch_instruction*)instruction);
        break;
    case instruction_type::plus_instruction_type:
        execute((plus_instruction*)instruction);
        break;
    case instruction_type::minus_instruction_type:
        execute((minus_instruction*)instruction);
        break;
    case instruction_type::call_instruction_type:
        execute((call_instruction*)instruction);
        break;
    case instruction_type::return_instruction_type:
        execute((return_instruction*)instruction);
        break;
    case instruction_type::push_instruction_type:
        execute((push_instruction*)instruction);
        break;
    case instruction_type::pop_instruction_type:
        execute((pop_instruction*)instruction);
        break;
    case instruction_type::print_instruction_type:
        execute((print_instruction*)instruction);
        break;
    case instruction_type::break_instruction_type:
        execute((break_instruction*)instruction);
        break;
    }
    this->ip++;
}

void virtual_machine_impl::execute(load_instruction* instruction)
{
    this->registers[instruction->destination_register] = data_memory[sp + instruction->location];
}

void virtual_machine_impl::execute(load_immediate_instruction* instruction)
{
    this->registers[instruction->destination_register] = instruction->value;
}

void virtual_machine_impl::execute(compare_instruction* instruction)
{
    this->registers[instruction->destination_register] = (this->registers[instruction->operand1] == this->registers[instruction->operand2]) ? 1 : 0;
}

void virtual_machine_impl::execute(store_instruction* instruction)
{
    data_memory[sp + instruction->location] = this->registers[instruction->source_register];
}

void virtual_machine_impl::execute(branch_on_zero_instruction* instruction)
{
    if (this->registers[instruction->operand] == 0)
    {
        // Because the main execute routine would add 1 to the IP, subtract 1 here to compensate for it.
        this->ip = instruction->branchTo->address - 1;
    }
}

void virtual_machine_impl::execute(branch_instruction* instruction)
{
    // Because the main execute routine would add 1 to the IP, subtract 1 here to compensate for it.
    this->ip = instruction->branchTo->address - 1;
}

void virtual_machine_impl::execute(plus_instruction* instruction)
{
    this->registers[instruction->destination_register] = this->registers[instruction->operand1] + this->registers[instruction->operand2];
}

void virtual_machine_impl::execute(minus_instruction* instruction)
{
    this->registers[instruction->destination_register] = this->registers[instruction->operand1] - this->registers[instruction->operand2];

}

void virtual_machine_impl::execute(call_instruction* instruction)
{
    this->data_memory[this->sp] = this->ip + 1;
    this->sp--;

    // Because the main execute routine would add 1 to the IP, subtract 1 here to compensate for it.
    this->ip = instruction->target->address - 1;
}

void virtual_machine_impl::execute(return_instruction* instruction)
{
    ++this->sp;

    // Because the main execute routine would add 1 to the IP, subtract 1 here to compensate for it.
    this->ip = this->data_memory[this->sp] - 1;
}

void virtual_machine_impl::execute(push_instruction* instruction)
{
    this->sp -= instruction->offset;
}

void virtual_machine_impl::execute(pop_instruction* instruction)
{
    this->sp += instruction->offset;
}

void virtual_machine_impl::execute(print_instruction* instruction)
{
    cout << this->registers[1] << endl;
}

void virtual_machine_impl::execute(break_instruction* instruction)
{
    this->break_instruction_executed = true;
}