#pragma once

#include "scanner.h"

struct program_node;
struct function_node;
struct statement_node;
struct if_statement_node;
struct condition_node;
struct value_node;
class parser_impl;

struct program_node
{
};

struct function_node
{
    function_node();
    ~function_node();
    char* function_name;
    char* argument_name;
    statement_node* statement;
};

struct statement_node
{
    virtual ~statement_node();
};

struct if_statement_node : public statement_node
{
    if_statement_node();
    ~if_statement_node();
    condition_node* condition;
    statement_node* true_statement;
    statement_node* false_statement;
};

struct return_statement_node : public statement_node
{
    return_statement_node();
    ~return_statement_node();
    value_node* value;
};

struct condition_node
{
    ~condition_node();
    char* variable_name;
    int value;
};

struct value_node
{
    virtual ~value_node();
};

class parser
{
public:
    parser(scanner* scanner);
    program_node* parse();
private:
    parser_impl* impl;
};