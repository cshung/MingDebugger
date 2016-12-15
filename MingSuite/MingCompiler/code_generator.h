#pragma once

#include "nodes.h"
#include "instructions.h"

class code_generator_impl;

class code_generator
{
public:
    code_generator();
    ~code_generator();
    instruction_sequence generate_code(program_node* program);
private:
    code_generator_impl* impl;
};
