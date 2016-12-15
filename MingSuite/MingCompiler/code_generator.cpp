#include "code_generator.h"

#include <unordered_map>
using namespace std;

struct code_generation_context
{
    code_generation_context();
    ~code_generation_context();
    unordered_map<string, instruction_sequence> functions;
    label_instruction* epilog_label;
    int tempUsed;
    int expressionTarget;
};

class code_generator_impl
{
public:
    code_generator_impl();
    ~code_generator_impl();
    instruction_sequence generate_code(program_node* program);
private:
    instruction_sequence generate_code(program_node* program, code_generation_context* context);
    instruction_sequence generate_code(function_node* function, code_generation_context* context);
    instruction_sequence generate_code(statement_node* statement, code_generation_context* context);
    instruction_sequence generate_code(if_statement_node* if_statement, code_generation_context* context);
    instruction_sequence generate_code(return_statement_node* return_statement, code_generation_context* context);
    instruction_sequence generate_code(call_statement_node* call_statement, code_generation_context* context);
    instruction_sequence generate_code(expression_node* expression_statement, code_generation_context* context);
    instruction_sequence generate_code(literal_node* literal, code_generation_context* context);
    instruction_sequence generate_code(identifier_node* variable, code_generation_context* context);
    instruction_sequence generate_code(call_node* call, code_generation_context* context);
    instruction_sequence generate_code(plus_node* plus, code_generation_context* context);
    instruction_sequence generate_code(minus_node* minus, code_generation_context* context);
    instruction_sequence generate_code(condition_node* condition, code_generation_context* context);
};

code_generator::code_generator()
{
    this->impl = new code_generator_impl();
}

code_generator::~code_generator()
{
    delete this->impl;
}

instruction_sequence code_generator::generate_code(program_node* program)
{
    return this->impl->generate_code(program);
}

code_generator_impl::code_generator_impl()
{
}

code_generator_impl::~code_generator_impl()
{
}

instruction_sequence code_generator_impl::generate_code(program_node* program)
{
    code_generation_context context;
    return this->generate_code(program, &context);
}

instruction_sequence code_generator_impl::generate_code(program_node* program, code_generation_context* context)
{
    instruction_sequence result;
    function_node* function = program->function;
    while (function != nullptr)
    {
        instruction_sequence function_body = this->generate_code(function, context);
        context->functions.insert(make_pair(function->function_name, function_body));
    }

    // TODO: Layout and linking
    return result;
}

instruction_sequence code_generator_impl::generate_code(function_node* function, code_generation_context* context)
{
    instruction_sequence result;
    label_instruction* epilog_label = new label_instruction();
    context->epilog_label = epilog_label;
    context->tempUsed = 0;
    instruction_sequence statement_body = generate_code(function->statement, context);

    // TODO: Generate prolog and epilog
    return result;
}

instruction_sequence code_generator_impl::generate_code(statement_node* statement, code_generation_context* context)
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
        instruction_sequence result;
        return result;
    }
}

instruction_sequence code_generator_impl::generate_code(if_statement_node* if_statement, code_generation_context* context)
{
    instruction_sequence result;
    int condition_target = ++context->tempUsed;
    instruction_sequence condition_code = this->generate_code(if_statement->condition, context);
    return result;
}

instruction_sequence code_generator_impl::generate_code(return_statement_node* return_statement, code_generation_context* context)
{
    instruction_sequence result;
    int value_target = ++context->tempUsed;
    context->expressionTarget = value_target;
    instruction_sequence value_code = this->generate_code(return_statement->value, context);

    // load r2, expression_target
    // jump epilog
    
    return value_code;
}

instruction_sequence code_generator_impl::generate_code(call_statement_node* call_statement, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(expression_node* expression, code_generation_context* context)
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
            instruction_sequence result;
            return result;
    }
}

instruction_sequence code_generator_impl::generate_code(literal_node* literal, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(identifier_node* variable, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(call_node* call, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(plus_node* plus, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(minus_node* minus, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

instruction_sequence code_generator_impl::generate_code(condition_node* condition, code_generation_context* context)
{
    instruction_sequence result;
    return result;
}

code_generation_context::code_generation_context()
{
    this->epilog_label = nullptr;
    this->tempUsed = 0;
}

code_generation_context::~code_generation_context()
{
    if (this->epilog_label != nullptr)
    {
        delete this->epilog_label;
    }
}

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