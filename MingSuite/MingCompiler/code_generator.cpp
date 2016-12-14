#include "code_generator.h"

class code_generator_impl
{
public:
    code_generator_impl();
    ~code_generator_impl();
    void generate_code(program_node* program);
private:
};

code_generator::code_generator()
{
    this->impl = new code_generator_impl();
}

code_generator::~code_generator()
{
    delete this->impl;
}

void code_generator::generate_code(program_node* program)
{
    this->impl->generate_code(program);
}

code_generator_impl::code_generator_impl()
{
}

code_generator_impl::~code_generator_impl()
{
}

void code_generator_impl::generate_code(program_node* program)
{
    // Do something!
}