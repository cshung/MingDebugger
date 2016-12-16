#include "instructions.h"
#include <iostream>
using namespace std;

instruction::instruction()
{
    this->next = nullptr;
}

instruction::~instruction()
{
}

instruction_sequence::instruction_sequence()
{
    this->head = nullptr;
    this->tail = nullptr;
}

int label_instruction::label_counter = 0;

label_instruction::label_instruction()
{
    this->label_id = ++label_counter;
    this->address = -1;
}

label_instruction::~label_instruction()
{

}

instruction_type label_instruction::get_instruction_type()
{
    return label_instruction_type;
}

void label_instruction::assign_address(int address)
{
    this->address = address;
}

void label_instruction::print()
{
    cout << "label: " << this->label_id << endl;
}

load_instruction::~load_instruction()
{
}

instruction_type load_instruction::get_instruction_type()
{
    return instruction_type::load_instruction_type;
}

void load_instruction::print()
{
    cout << "load sp+" << this->location << " to R" << this->destination_register << endl;
}

load_immediate_instruction::~load_immediate_instruction()
{
}

instruction_type load_immediate_instruction::get_instruction_type()
{
    return instruction_type::load_immediate_instruction_type;
}

void load_immediate_instruction::print()
{
    cout << "load value " << this->value << " to R" << this->destination_register << endl;
}

compare_instruction::~compare_instruction()
{
}

instruction_type compare_instruction::get_instruction_type()
{
    return instruction_type::compare_instruction_type;
}

void compare_instruction::print()
{
    cout << "compare R" << this->operand1 << " and R" << this->operand2 << " and store result to R" << this->destination_register << endl;
}

store_instruction::~store_instruction()
{
}

instruction_type store_instruction::get_instruction_type()
{
    return instruction_type::store_instruction_type;
}

void store_instruction::print()
{
    cout << "storing R" << this->source_register << " to sp+" << this->location << endl;
}

branch_on_zero_instruction::~branch_on_zero_instruction()
{
}

instruction_type branch_on_zero_instruction::get_instruction_type()
{
    return instruction_type::branch_on_zero_instruction_type;
}

void branch_on_zero_instruction::print()
{
    cout << "branch to " << this->branchTo->address << " if R" << this->operand << " is 0" << endl;
}

branch_instruction::~branch_instruction()
{
}

instruction_type branch_instruction::get_instruction_type()
{
    return instruction_type::branch_instruction_type;
}

void branch_instruction::print()
{
    cout << "branch to " << this->branchTo->address << endl;
}

plus_instruction::~plus_instruction()
{
}

instruction_type plus_instruction::get_instruction_type()
{
    return instruction_type::plus_instruction_type;
}

void plus_instruction::print()
{
    cout << "Set R" << this->destination_register << " to R" << this->operand1 << " + R" << this->operand2 << endl;
}

minus_instruction::~minus_instruction()
{
}

instruction_type minus_instruction::get_instruction_type()
{
    return instruction_type::minus_instruction_type;
}

void minus_instruction::print()
{
    cout << "Set R" << this->destination_register << " to R" << this->operand1 << " - R" << this->operand2 << endl;
}

call_instruction::~call_instruction()
{
}

instruction_type call_instruction::get_instruction_type()
{
    return instruction_type::call_instruction_type;
}

void call_instruction::print()
{
    cout << "call " << this->target->address << endl;
}

return_instruction::~return_instruction()
{
}

instruction_type return_instruction::get_instruction_type()
{
    return instruction_type::return_instruction_type;
}

void return_instruction::print()
{
    cout << "return" << endl;
}

push_instruction::~push_instruction()
{
}

instruction_type push_instruction::get_instruction_type()
{
    return instruction_type::push_instruction_type;
}

void push_instruction::print()
{
    cout << "sp = sp - " << offset << endl;
}

pop_instruction::~pop_instruction()
{
}

instruction_type pop_instruction::get_instruction_type()
{
    return instruction_type::pop_instruction_type;
}

void pop_instruction::print()
{
    cout << "sp = sp + " << offset << endl;
}

print_instruction::~print_instruction()
{
}

instruction_type print_instruction::get_instruction_type()
{
    return instruction_type::print_instruction_type;
}

void print_instruction::print()
{
    cout << "print R1" << endl;
}