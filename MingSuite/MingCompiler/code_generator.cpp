#include "code_generator.h"

#include <unordered_map>
using namespace std;

struct code_generation_context
{
    unordered_map<string, instruction*> functions;
    label_instruction* epilog_label;
};

class code_generator_impl
{
public:
    code_generator_impl();
    ~code_generator_impl();
    instruction* generate_code(program_node* program);
private:
    instruction* generate_code(program_node* program, code_generation_context* context);
    instruction* generate_code(function_node* function, code_generation_context* context);
    instruction* generate_code(statement_node* statement, code_generation_context* context);
};

code_generator::code_generator()
{
    this->impl = new code_generator_impl();
}

code_generator::~code_generator()
{
    delete this->impl;
}

instruction* code_generator::generate_code(program_node* program)
{
    return this->impl->generate_code(program);
}

code_generator_impl::code_generator_impl()
{
}

code_generator_impl::~code_generator_impl()
{
}

instruction* code_generator_impl::generate_code(program_node* program)
{
    code_generation_context context;
    return this->generate_code(program, &context);
}

instruction* code_generator_impl::generate_code(program_node* program, code_generation_context* context)
{
    function_node* function = program->function;
    while (function != nullptr)
    {
        instruction* function_body = this->generate_code(function, context);
        context->functions.insert(make_pair(function->function_name, function_body));
    }

    // TODO: Layout and linking
    return nullptr;
}

instruction* code_generator_impl::generate_code(function_node* function, code_generation_context* context)
{
    label_instruction* epilog_label = new label_instruction();
    context->epilog_label = epilog_label;
    instruction* statement_body = generate_code(function->statement, context);
    return nullptr;
}

instruction* code_generator_impl::generate_code(statement_node* function, code_generation_context* context)
{
    // TODO: Implementation
    return nullptr;
}

instruction::instruction()
{
    this->next = nullptr;
}

instruction::~instruction()
{
    if (this->next != nullptr)
    {
        delete this->next;
    }
}

label_instruction::~label_instruction()
{
}