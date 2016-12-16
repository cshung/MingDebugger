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
}

label_instruction::~label_instruction()
{
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
    cout << "load sp-" << this->location << " to R" << this->destination_register;
}

load_immediate_instruction::~load_immediate_instruction()
{
}

void load_immediate_instruction::print()
{
    cout << "load value " << this->value << " to R" << this->destination_register;
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
    cout << "storing R" << this->source_register << "sp-" << this->location << endl;
}

branch_on_zero_instruction::~branch_on_zero_instruction()
{
}

branch_instruction::~branch_instruction()
{
}

plus_instruction::~plus_instruction()
{
}

minus_instruction::~minus_instruction()
{
}

call_instruction::~call_instruction()
{
}

return_instruction::~return_instruction()
{
}

push_instruction::~push_instruction()
{
}

pop_instruction::~pop_instruction()
{
}
