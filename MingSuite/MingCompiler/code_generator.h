#pragma once

#include "nodes.h"

class code_generator_impl;

class code_generator
{
public:
    code_generator();
    ~code_generator();
    void generate_code(program_node* program);
private:
    code_generator_impl* impl;
};
