#include "instructions.h"

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

label_instruction::~label_instruction()
{
}

load_instruction::~load_instruction()
{
}

load_immediate_instruction::~load_immediate_instruction()
{
}

compare_instruction::~compare_instruction()
{
}

store_instruction::~store_instruction()
{
}
