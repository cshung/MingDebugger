#include "nodes.h"

program_node::~program_node()
{
    if (this->function != nullptr)
    {
        delete this->function;
    }
}

function_node::function_node()
{
    this->function_name = nullptr;
    this->argument_name = nullptr;
    this->next_function = nullptr;
}

function_node::~function_node()
{
    if (this->function_name != nullptr)
    {
        delete[] function_name;
    }
    if (this->argument_name != nullptr)
    {
        delete[] argument_name;
    }
    if (this->next_function != nullptr)
    {
        delete this->next_function;
    }
}

statement_node::~statement_node()
{

}

if_statement_node::if_statement_node()
{
    this->condition = nullptr;
    this->true_statement = nullptr;
    this->false_statement = nullptr;
}

if_statement_node::~if_statement_node()
{
    if (this->condition != nullptr)
    {
        delete this->condition;
    }
    if (this->true_statement != nullptr)
    {
        delete this->true_statement;
    }
    if (this->false_statement != nullptr)
    {
        delete this->false_statement;
    }
}

call_statement_node::call_statement_node()
{
    this->function_name = nullptr;
    this->argument = nullptr;
}

call_statement_node::~call_statement_node()
{
    if (this->function_name != nullptr)
    {
        delete[] this->function_name;
    }
    if (this->argument != nullptr)
    {
        delete this->argument;
    }
}

condition_node::condition_node()
{
    this->variable_name = nullptr;
}

condition_node::~condition_node()
{
    if (this->variable_name != nullptr)
    {
        delete[] this->variable_name;
    }
}

return_statement_node::return_statement_node()
{
    this->value = nullptr;
}

return_statement_node::~return_statement_node()
{
    if (this->value != nullptr)
    {
        delete this->value;
    }
}

expression_node::~expression_node()
{
}

term_node::~term_node()
{
}

literal_node::~literal_node()
{
}

identifier_node::identifier_node()
{
    this->identifier_name = nullptr;
}

identifier_node::~identifier_node()
{
    if (this->identifier_name != nullptr)
    {
        delete[] this->identifier_name;
    }
}

call_node::call_node()
{
    this->function_name = nullptr;
    this->argument = nullptr;
}

call_node::~call_node()
{
    if (this->function_name != nullptr)
    {
        delete[] this->function_name;
    }
    if (this->argument)
    {
        delete this->argument;
    }
}

plus_node::plus_node()
{
    this->left = nullptr;
    this->right = nullptr;
}

plus_node::~plus_node()
{
    if (this->left)
    {
        delete this->left;
    }
    if (this->right)
    {
        delete this->right;
    }
}

minus_node::minus_node()
{
    this->left = nullptr;
    this->right = nullptr;
}

minus_node::~minus_node()
{
    if (this->left)
    {
        delete this->left;
    }
    if (this->right)
    {
        delete this->right;
    }
}