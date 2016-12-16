#include "virtual_machine.h"
#include <vector>
#include <iostream>
using namespace std;

class virtual_machine_impl
{
public:
    void run(instruction_sequence instructions, int entry_point);
private:
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
    vector<instruction*> code_memory;
    vector<int> data_memory;
    int ip;
    int sp;
    int registers[5];
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

void virtual_machine_impl::run(instruction_sequence instructions, int entry_point)
{
    data_memory.resize(256);
    instruction* cursor = instructions.head;
    while (cursor != nullptr)
    {
        this->code_memory.push_back(cursor);
        cursor = cursor->next;
    }
    sp = 254;
    data_memory[255] = -1;
    ip = entry_point;
    while (true)
    {
        instruction* current = code_memory[this->ip];
        current->print();
        this->execute(current);
        cout << "R1 = " << this->registers[1] << endl;
        cout << "R2 = " << this->registers[2] << endl;
        cout << "R3 = " << this->registers[3] << endl;
        cout << "R4 = " << this->registers[4] << endl;
        cout << "SP = " << this->sp << endl;
        cout << "IP = " << this->ip << endl;
    }
}

void virtual_machine_impl::execute(instruction* instruction)
{
    switch (instruction->get_instruction_type())
    {
    case instruction_type::load:
        execute((load_instruction*)instruction);
        break;
    case instruction_type::load_immediate:
        execute((load_immediate_instruction*)instruction);
        break;
    case instruction_type::compare:
        execute((compare_instruction*)instruction);
        break;
    case instruction_type::store:
        execute((store_instruction*)instruction);
        break;
    case instruction_type::branch_on_zero:
        execute((branch_on_zero_instruction*)instruction);
        break;
    case instruction_type::branch:
        execute((branch_instruction*)instruction);
        break;
    case instruction_type::plus:
        execute((plus_instruction*)instruction);
        break;
    case instruction_type::minus:
        execute((minus_instruction*)instruction);
        break;
    case instruction_type::call:
        execute((call_instruction*)instruction);
        break;
    case instruction_type::return_instruction_type:
        execute((return_instruction*)instruction);
        break;
    case instruction_type::push:
        execute((push_instruction*)instruction);
        break;
    case instruction_type::pop:
        execute((pop_instruction*)instruction);
        break;
    case instruction_type::print:
        execute((print_instruction*)instruction);
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
        this->ip = instruction->branchTo->address - 1;
    }
}

void virtual_machine_impl::execute(branch_instruction* instruction)
{
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
    this->data_memory[this->sp] = this->ip;
    this->sp--;
    this->ip = instruction->target->address - 1;
}

void virtual_machine_impl::execute(return_instruction* instruction)
{
    this->ip = this->data_memory[this->sp];
    ++this->sp;
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
