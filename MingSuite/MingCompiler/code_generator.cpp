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
    instruction* generate_code(if_statement_node* if_statement, code_generation_context* context);
    instruction* generate_code(return_statement_node* return_statement, code_generation_context* context);
    instruction* generate_code(call_statement_node* call_statement, code_generation_context* context);
    instruction* generate_code(expression_node* expression_statement, code_generation_context* context);
    instruction* generate_code(literal_node* literal, code_generation_context* context);
    instruction* generate_code(identifier_node* variable, code_generation_context* context);
    instruction* generate_code(call_node* call, code_generation_context* context);
    instruction* generate_code(plus_node* plus, code_generation_context* context);
    instruction* generate_code(minus_node* minus, code_generation_context* context);
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

    // TODO: Generate prolog and epilog
    return nullptr;
}

instruction* code_generator_impl::generate_code(statement_node* statement, code_generation_context* context)
{
    switch (statement->get_statement_node_type())
    {
    case if_statement:
        return this->generate_code((if_statement_node*)statement, context);
    case return_statement:
        return this->generate_code((return_statement_node*)statement, context);
    case call_statement:
        return this->generate_code((call_statement_node*)statement, context);
    default:
        break;
    }
    return nullptr;
}

instruction* code_generator_impl::generate_code(if_statement_node* if_statement, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(return_statement_node* return_statement, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(call_statement_node* call_statement, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(expression_node* expression, code_generation_context* context)
{
    switch (expression->get_expression_node_type())
    {
        case literal:
            return this->generate_code((literal_node*)expression, context);
        case variable:
            return this->generate_code((identifier_node*)expression, context);
        case call:
            return this->generate_code((call_node*)expression, context);
        case plus_node_type:
            return this->generate_code((plus_node*)expression, context);
        case minus_node_type:
            return this->generate_code((minus_node*)expression, context);
        default:
            return nullptr;
    }
}

instruction* code_generator_impl::generate_code(literal_node* literal, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(identifier_node* variable, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(call_node* call, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(plus_node* plus, code_generation_context* context)
{
    return nullptr;
}

instruction* code_generator_impl::generate_code(minus_node* minus, code_generation_context* context)
{
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