#pragma once

struct program_node;
struct function_node;
struct statement_node;
struct if_statement_node;
struct condition_node;
struct expression_node;
class parser_impl;

struct parse_node
{
    int begin_line;
    int begin_column;
    int end_line;
    int end_column;
};

struct program_node : parse_node
{
    ~program_node();

    function_node* function;
};

struct function_node : parse_node
{
    function_node();
    ~function_node();

    char* function_name;
    char* argument_name;
    statement_node* statement;
    function_node* next_function;
};

enum statement_node_type
{
    if_statement,
    return_statement,
    call_statement
};

struct statement_node : parse_node
{
    virtual ~statement_node();
    virtual statement_node_type get_statement_node_type() = 0;
};

struct if_statement_node : public statement_node
{
    if_statement_node();
    ~if_statement_node();
    virtual statement_node_type get_statement_node_type();

    condition_node* condition;
    statement_node* true_statement;
    statement_node* false_statement;
};

struct return_statement_node : public statement_node
{
    return_statement_node();
    ~return_statement_node();
    virtual statement_node_type get_statement_node_type();

    expression_node* value;
};

struct call_statement_node : public statement_node
{
    call_statement_node();
    ~call_statement_node();
    virtual statement_node_type get_statement_node_type();

    char* function_name;
    expression_node* argument;
};

struct condition_node : parse_node
{
    condition_node();
    ~condition_node();
    char* variable_name;
    int value;
};

enum expression_node_type
{
    literal_expression,
    variable_expression,
    call_expression,
    plus_expression,
    minus_expression
};

struct expression_node : parse_node
{
    virtual ~expression_node();
    virtual expression_node_type get_expression_node_type() = 0;
};

struct term_node : public expression_node
{
    virtual ~term_node();
    virtual expression_node_type get_expression_node_type() = 0;
};

struct literal_node : public term_node
{
    virtual ~literal_node();
    virtual expression_node_type get_expression_node_type();
    
    int value;
};

struct identifier_node : public term_node
{
    identifier_node();
    virtual ~identifier_node();
    virtual expression_node_type get_expression_node_type();

    char* identifier_name;
};

struct call_node : public term_node
{
    call_node();
    virtual ~call_node();
    char* function_name;
    virtual expression_node_type get_expression_node_type();

    expression_node* argument;
};

struct plus_node : public expression_node
{
    plus_node();
    virtual ~plus_node();
    virtual expression_node_type get_expression_node_type();

    expression_node* left;
    term_node* right;
};

struct minus_node : public expression_node
{
    minus_node();
    virtual ~minus_node();
    virtual expression_node_type get_expression_node_type();

    expression_node* left;
    term_node* right;
};
