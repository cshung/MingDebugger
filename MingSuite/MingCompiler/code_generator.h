#pragma once

#include "nodes.h"
#include "instructions.h"
#include "symbols.h"

class code_generator_impl;

struct code_generation_outputs
{
    instruction_sequence instructions;
    int entry_point;
    struct symbols symbols;
};

class code_generator
{
public:
    code_generator();
    ~code_generator();
    code_generation_outputs generate_code(program_node* program);
private:
    code_generator_impl* impl;
};
