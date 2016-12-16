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

bool instruction::is_label()
{
    return false;
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

bool label_instruction::is_label()
{
    return true;
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

void load_instruction::print()
{
    cout << "load sp+" << this->location << " to R" << this->destination_register << endl;
}

load_immediate_instruction::~load_immediate_instruction()
{
}

void load_immediate_instruction::print()
{
    cout << "load value " << this->value << " to R" << this->destination_register << endl;
}

compare_instruction::~compare_instruction()
{
}

void compare_instruction::print()
{
    cout << "compare R" << this->operand1 << " and R" << this->operand2 << " and store result to R" << this->destination_register << endl;
}

store_instruction::~store_instruction()
{
}

void store_instruction::print()
{
    cout << "storing R" << this->source_register << " to sp+" << this->location << endl;
}

branch_on_zero_instruction::~branch_on_zero_instruction()
{
}

void branch_on_zero_instruction::print()
{
    cout << "branch to " << this->branchTo->address << " if R" << this->operand << " is 0" << endl;
}

branch_instruction::~branch_instruction()
{
}

void branch_instruction::print()
{
    cout << "branch to " << this->branchTo->address << endl;
}

plus_instruction::~plus_instruction()
{
}

void plus_instruction::print()
{
    cout << "R" << this->destination_register << " = R" << this->operand1 << " + R" << this->operand2 << endl;
}

minus_instruction::~minus_instruction()
{
}

void minus_instruction::print()
{
    cout << "R" << this->destination_register << " = R" << this->operand1 << " - R" << this->operand2 << endl;
}

call_instruction::~call_instruction()
{
}

void call_instruction::print()
{
    cout << "call " << this->target->address << endl;
}

return_instruction::~return_instruction()
{
}

void return_instruction::print()
{
    cout << "return" << endl;
}

push_instruction::~push_instruction()
{
}

void push_instruction::print()
{
    cout << "sp = sp - " << offset << endl;
}

pop_instruction::~pop_instruction()
{
}

void pop_instruction::print()
{
    cout << "sp = sp + " << offset << endl;
}

print_instruction::~print_instruction()
{
}

void print_instruction::print()
{
    cout << "print R1" << endl;
}