#pragma once

struct program_node;
struct function_node;
struct statement_node;
struct if_statement_node;
struct condition_node;
struct expression_node;
class parser_impl;

struct program_node
{
    ~program_node();
    function_node* function;
};

struct function_node
{
    function_node();
    ~function_node();
    char* function_name;
    char* argument_name;
    statement_node* statement;
    function_node* next_function;
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
    expression_node* value;
};

struct call_statement_node : public statement_node
{
    call_statement_node();
    ~call_statement_node();
    char* function_name;
    expression_node* argument;
};

struct condition_node
{
    condition_node();
    ~condition_node();
    char* variable_name;
    int value;
};

struct expression_node
{
    virtual ~expression_node();
};

struct term_node : public expression_node
{
    virtual ~term_node();
};

struct literal_node : public term_node
{
    virtual ~literal_node();
    int value;
};

struct identifier_node : public term_node
{
    identifier_node();
    virtual ~identifier_node();
    char* identifier_name;
};

struct call_node : public term_node
{
    call_node();
    virtual ~call_node();
    char* function_name;
    expression_node* argument;
};

struct plus_node : public expression_node
{
    plus_node();
    virtual ~plus_node();
    expression_node* left;
    term_node* right;
};

struct minus_node : public expression_node
{
    minus_node();
    virtual ~minus_node();
    expression_node* left;
    term_node* right;
};
