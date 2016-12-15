#pragma once

#include "nodes.h"

class code_generator_impl;

struct instruction
{
    instruction();
    virtual ~instruction();
    instruction* next;
};

struct label_instruction : instruction
{
    virtual ~label_instruction();
};

class code_generator
{
public:
    code_generator();
    ~code_generator();
    instruction* generate_code(program_node* program);
private:
    code_generator_impl* impl;
};
