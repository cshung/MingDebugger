#include "code_generator.h"

#include <unordered_map>
using namespace std;

struct code_generation_context
{
    code_generation_context();
    ~code_generation_context();
    unordered_map<string, label_instruction*> function_labels;
    unordered_map<string, int> variables;
    label_instruction* epilog_label;
    int tempUsed;
    int expressionTarget;
};

class code_generator_impl
{
public:
    code_generator_impl();
    ~code_generator_impl();
    code_generation_outputs generate_code(program_node* program);
private:
    code_generation_outputs generate_code(program_node* program, code_generation_context* context);
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

void concatenate(instruction* prev, instruction* next)
{
    prev->next = next;
    next->prev = prev;
}

void concatenate(instruction_sequence prev, instruction* next)
{
    prev.tail->next = next;
    next->prev = prev.tail;
}

void concatenate(instruction* prev, instruction_sequence next)
{
    prev->next = next.head;
    next.head->prev = prev;
}

void concatenate(instruction_sequence prev, instruction_sequence next)
{
    prev.tail->next = next.head;
    next.head->prev = prev.tail;
}

code_generator::code_generator()
{
    this->impl = new code_generator_impl();
}

code_generator::~code_generator()
{
    delete this->impl;
}

code_generation_outputs code_generator::generate_code(program_node* program)
{
    return this->impl->generate_code(program);
}

code_generator_impl::code_generator_impl()
{
}

code_generator_impl::~code_generator_impl()
{
}

code_generation_outputs code_generator_impl::generate_code(program_node* program)
{
    code_generation_context context;
    return this->generate_code(program, &context);
}

code_generation_outputs code_generator_impl::generate_code(program_node* program, code_generation_context* context)
{
    instruction_sequence result;

    label_instruction* begin_program = new label_instruction();
    label_instruction* end_program = new label_instruction();

    function_node* function = program->function;
    while (function != nullptr)
    {
        label_instruction* label = new label_instruction();
        context->function_labels.insert(make_pair(function->function_name, label));
        function = function->next_function;
    }
    function = program->function;

    instruction* end = begin_program;
    while (function != nullptr)
    {
        instruction_sequence function_body = this->generate_code(function, context);
        concatenate(end, function_body);
        end = function_body.tail;
        function = function->next_function;
    }
    concatenate(end, end_program);

    // Removing the labels and replacing them as addresses
    instruction* cursor = begin_program->next;
    int address = 0;
    while (cursor != end_program)
    {
        if (cursor->get_instruction_type() == instruction_type::label_instruction_type)
        {
            label_instruction* label = (label_instruction*)cursor;
            label->assign_address(address);
            cursor = cursor->next;
            label->prev->next = label->next;
            label->next->prev = label->prev;
        }
        else
        {
            cursor = cursor->next;
            address++;
        }
    }

    result.head = begin_program->next;
    result.tail = end_program->prev;

    result.head->prev = nullptr;
    result.tail->next = nullptr;

    code_generation_outputs output;

    output.instructions = result;
    output.entry_point = context->function_labels["main"]->address;

    return output;
}

instruction_sequence code_generator_impl::generate_code(function_node* function, code_generation_context* context)
{
    instruction_sequence result;
    label_instruction* epilog_label = new label_instruction();
    context->epilog_label = epilog_label;
    context->tempUsed = 0;
    store_instruction* store_argument = nullptr;
    if (function->argument_name != nullptr)
    {
        int argument_location = ++context->tempUsed;
        store_argument = new store_instruction();
        store_argument->source_register = 1;
        store_argument->location = argument_location;
        context->variables.insert(make_pair(function->argument_name, argument_location));
    }
    instruction_sequence statement_body = generate_code(function->statement, context);

    push_instruction* push = new push_instruction();
    push->offset = context->tempUsed;

    pop_instruction* pop = new pop_instruction();
    pop->offset = context->tempUsed;

    return_instruction* return_op = new return_instruction();

    label_instruction* function_label = context->function_labels[function->function_name];

    result.head = function_label;

    concatenate(function_label, push);

    if (store_argument == nullptr)
    {
        concatenate(push, statement_body);
    }
    else
    {
        concatenate(push, store_argument);
        concatenate(store_argument, statement_body);
    }

    concatenate(statement_body, epilog_label);

    concatenate(epilog_label, pop);
    concatenate(pop, return_op);
    result.tail = return_op;

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
    context->expressionTarget = condition_target;
    instruction_sequence condition_code = this->generate_code(if_statement->condition, context);

    label_instruction* false_label = new label_instruction();
    label_instruction* complete_label = new label_instruction();

    // condition code
    // load r3, condition_target
    // beqz r3, false_label
    // true code
    // b complete
    // false_label
    // false code
    // complete_label
    load_instruction* load_condition = new load_instruction();
    load_condition->destination_register = 3;
    load_condition->location = condition_target;

    branch_on_zero_instruction* branch_to_false = new branch_on_zero_instruction();
    branch_to_false->operand = 3;
    branch_to_false->branchTo = false_label;

    instruction_sequence true_code = this->generate_code(if_statement->true_statement, context);
    
    branch_instruction* branch_to_complete = new branch_instruction();
    branch_to_complete->branchTo = complete_label;
    
    instruction_sequence false_code = this->generate_code(if_statement->false_statement, context);

    result.head = condition_code.head;
    concatenate(condition_code, load_condition);
    concatenate(load_condition, branch_to_false);
    concatenate(branch_to_false, true_code);
    concatenate(true_code, branch_to_complete);
    concatenate(branch_to_complete, false_label);
    concatenate(false_label, false_code);
    concatenate(false_code, complete_label);
    result.tail = complete_label;

    return result;
}

instruction_sequence code_generator_impl::generate_code(return_statement_node* return_statement, code_generation_context* context)
{
    instruction_sequence result;
    int value_target = ++context->tempUsed;
    context->expressionTarget = value_target;
    instruction_sequence value_code = this->generate_code(return_statement->value, context);

    // load r2, expression_target
    load_instruction* load_value = new load_instruction();
    load_value->destination_register = 2;
    load_value->location = value_target;

    // jump epilog
    branch_instruction* branch_to_epilog = new branch_instruction();
    branch_to_epilog->branchTo = context->epilog_label;

    result.head = value_code.head;
    concatenate(value_code, load_value);
    concatenate(load_value, branch_to_epilog);
    result.tail = branch_to_epilog;

    return result;
}

instruction_sequence code_generator_impl::generate_code(call_statement_node* call_statement, code_generation_context* context)
{
    instruction_sequence result;
    int argumentTarget = ++context->tempUsed;
    context->expressionTarget = argumentTarget;
    instruction_sequence argument_code = this->generate_code(call_statement->argument, context);
    load_instruction* load_argument = new load_instruction();
    load_argument->location = argumentTarget;
    load_argument->destination_register = 1;

    instruction* last_op = nullptr;
    if (strcmp(call_statement->function_name, "print") == 0)
    {
        last_op = new print_instruction();
    }
    else
    {
        call_instruction* call_op = new call_instruction();
        call_op->target = context->function_labels[call_statement->function_name];
        last_op = call_op;
    }

    result.head = argument_code.head;
    concatenate(argument_code, load_argument);
    concatenate(load_argument, last_op);
    result.tail = last_op;

    return result;
}

instruction_sequence code_generator_impl::generate_code(expression_node* expression, code_generation_context* context)
{
    switch (expression->get_expression_node_type())
    {
    case literal_expression:
        return this->generate_code((literal_node*)expression, context);
    case variable_expression:
        return this->generate_code((identifier_node*)expression, context);
    case call_expression:
        return this->generate_code((call_node*)expression, context);
    case plus_expression:
        return this->generate_code((plus_node*)expression, context);
    case minus_expression:
        return this->generate_code((minus_node*)expression, context);
    default:
        instruction_sequence result;
        return result;
    }
}

instruction_sequence code_generator_impl::generate_code(literal_node* literal, code_generation_context* context)
{
    instruction_sequence result;

    load_immediate_instruction* load_literal = new load_immediate_instruction();
    load_literal->destination_register = 4;
    load_literal->value = literal->value;

    store_instruction* store = new store_instruction();
    store->location = context->expressionTarget;
    store->source_register = 4;

    result.head = load_literal;
    concatenate(load_literal, store);
    result.tail = store;

    return result;
}

instruction_sequence code_generator_impl::generate_code(identifier_node* variable, code_generation_context* context)
{
    instruction_sequence result;
    load_instruction* load = new load_instruction();
    load->location = context->variables[variable->identifier_name];
    load->destination_register = 3;
    store_instruction* store = new store_instruction();
    store->source_register = 3;
    store->location = context->expressionTarget;
    
    result.head = load;
    concatenate(load, store);
    result.tail = store;

    return result;
}

instruction_sequence code_generator_impl::generate_code(call_node* call, code_generation_context* context)
{
    instruction_sequence result;
    int expressionTarget = context->expressionTarget;
    int argumentTarget = ++context->tempUsed;
    context->expressionTarget = argumentTarget;
    instruction_sequence argument_code = this->generate_code(call->argument, context);
    load_instruction* load_argument = new load_instruction();
    load_argument->location = argumentTarget;
    load_argument->destination_register = 1;
    call_instruction* call_op = new call_instruction();
    call_op->target = context->function_labels[call->function_name];
    store_instruction* store = new store_instruction();
    store->source_register = 2;
    store->location = expressionTarget;

    result.head = argument_code.head;
    concatenate(argument_code, load_argument);
    concatenate(load_argument, call_op);
    concatenate(call_op, store);
    result.tail = store;

    return result;
}

instruction_sequence code_generator_impl::generate_code(plus_node* plus, code_generation_context* context)
{
    instruction_sequence result;
    int expressionTarget = context->expressionTarget;
    int left_target = ++context->tempUsed;
    int right_target = ++context->tempUsed;
    context->expressionTarget = left_target;
    instruction_sequence left_code = this->generate_code(plus->left, context);
    context->expressionTarget = right_target;
    instruction_sequence right_code = this->generate_code(plus->right, context);
    load_instruction* load_left = new load_instruction();
    load_left->destination_register = 3;
    load_left->location = left_target;
    load_instruction* load_right = new load_instruction();
    load_right->destination_register = 4;
    load_right->location = right_target;
    plus_instruction* plus_op = new plus_instruction();
    plus_op->destination_register = 2;
    plus_op->operand1 = 3;
    plus_op->operand2 = 4;
    store_instruction* store = new store_instruction();
    store->source_register = 2;
    store->location = expressionTarget;
    result.head = left_code.head;
    concatenate(left_code, right_code);
    concatenate(right_code, load_left);
    concatenate(load_left, load_right);
    concatenate(load_right, plus_op);
    concatenate(plus_op, store);
    result.tail = store;
    return result;
}

instruction_sequence code_generator_impl::generate_code(minus_node* minus, code_generation_context* context)
{
    instruction_sequence result;
    int expressionTarget = context->expressionTarget;
    int left_target = ++context->tempUsed;
    int right_target = ++context->tempUsed;
    context->expressionTarget = left_target;
    instruction_sequence left_code = this->generate_code(minus->left, context);
    context->expressionTarget = right_target;
    instruction_sequence right_code = this->generate_code(minus->right, context);
    load_instruction* load_left = new load_instruction();
    load_left->destination_register = 3;
    load_left->location = left_target;
    load_instruction* load_right = new load_instruction();
    load_right->destination_register = 4;
    load_right->location = right_target;
    minus_instruction* minus_op = new minus_instruction();
    minus_op->destination_register = 2;
    minus_op->operand1 = 3;
    minus_op->operand2 = 4;
    store_instruction* store = new store_instruction();
    store->source_register = 2;
    store->location = expressionTarget;
    result.head = left_code.head;
    concatenate(left_code, right_code);
    concatenate(right_code, load_left);
    concatenate(load_left, load_right);
    concatenate(load_right, minus_op);
    concatenate(minus_op, store);
    result.tail = store;
    return result;
}

instruction_sequence code_generator_impl::generate_code(condition_node* condition, code_generation_context* context)
{
    instruction_sequence result;
    int variable_location = context->variables[condition->variable_name];
    int literal = condition->value;
    // load r3, variable_location
    load_instruction* load_variable = new load_instruction();
    load_variable->destination_register = 3;
    load_variable->location = variable_location;

    // loadimm r4, literal
    load_immediate_instruction* load_literal = new load_immediate_instruction();
    load_literal->destination_register = 4;
    load_literal->value = literal;

    // cmp r2, r3, r4
    compare_instruction* compare = new compare_instruction();
    compare->destination_register = 2;
    compare->operand1 = 3;
    compare->operand2 = 4;

    // store target, r2
    store_instruction* store = new store_instruction();
    store->location = context->expressionTarget;
    store->source_register = 2;

    result.head = load_variable;
    concatenate(load_variable, load_literal);
    concatenate(load_literal, compare);
    concatenate(compare, store);
    result.tail = store;

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
