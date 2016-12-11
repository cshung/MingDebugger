#include "parser.h"

class parser_impl
{
public:
    parser_impl(scanner* scanner);
    program_node* parse();
private:
    scanner* m_scanner;

    program_node* parse_program();
    function_node* parse_function();
    statement_node* parse_statement();
    condition_node* parse_condition();
};

parser::parser(scanner* scanner)
{
    this->impl = new parser_impl(scanner);
}

program_node* parser::parse()
{
    return this->impl->parse();
}

parser_impl::parser_impl(scanner* scanner) : m_scanner(scanner)
{
}

program_node* parser_impl::parse()
{
    this->m_scanner->scan();
    return this->parse_program();
}

program_node* parser_impl::parse_program()
{
    program_node* result = new program_node();
    parse_function();
    // TODO: Parse more functions
    return result;
}

function_node* parser_impl::parse_function()
{
    function_node* result = new function_node();
    if (this->m_scanner->get_token_type() == function)
    {
        this->m_scanner->scan();
        if (this->m_scanner->get_token_type() == identifier)
        {
            result->function_name = this->m_scanner->get_token_string();
            this->m_scanner->scan();
            if (this->m_scanner->get_token_type() == left_bracket)
            {
                this->m_scanner->scan();
                if (this->m_scanner->get_token_type() == identifier)
                {
                    result->argument_name = this->m_scanner->get_token_string();
                    this->m_scanner->scan();
                    if (this->m_scanner->get_token_type() == right_bracket)
                    {
                        this->m_scanner->scan();
                        if (this->m_scanner->get_token_type() == left_brace)
                        {
                            this->m_scanner->scan();
                            result->statement = this->parse_statement();
                            if (result->statement != nullptr)
                            {
                                this->m_scanner->scan();
                                if (this->m_scanner->get_token_type() == right_brace)
                                {
                                    this->m_scanner->scan();
                                    return result;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    delete result;
    return nullptr;
}

statement_node* parser_impl::parse_statement()
{
    statement_node* result = nullptr;
    if (this->m_scanner->get_token_type() == _if)
    {
        if_statement_node* if_result = new if_statement_node();
        result = if_result;
        this->m_scanner->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            this->m_scanner->scan();
            if_result->condition = parse_condition();
            if (if_result->condition != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->m_scanner->scan();
                    if (this->m_scanner->get_token_type() == left_brace)
                    {
                        this->m_scanner->scan();
                        if_result->true_statement = parse_statement();
                        if (if_result->true_statement != nullptr)
                        {
                            if (this->m_scanner->get_token_type() == right_brace)
                            {
                                this->m_scanner->scan();
                                if (this->m_scanner->get_token_type() == _else)
                                {
                                    this->m_scanner->scan();
                                    if_result->false_statement = parse_statement();
                                    if (if_result->false_statement != nullptr)
                                    {
                                        if (this->m_scanner->get_token_type() == right_brace)
                                        {
                                            this->m_scanner->scan();
                                            return if_result;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (this->m_scanner->get_token_type() == _return)
    {
        // TODO: Work on the return statement!
    }
    if (result != nullptr)
    {
        delete result;
    }
    return nullptr;
}

condition_node* parser_impl::parse_condition()
{
    condition_node* result = new condition_node();
    if (this->m_scanner->get_token_type() == identifier)
    {
        result->variable_name = this->m_scanner->get_token_string();
        this->m_scanner->scan();
        if (this->m_scanner->get_token_type() == equals)
        {
            this->m_scanner->scan();
            if (this->m_scanner->get_token_type() == equals)
            {
                this->m_scanner->scan();
                if (this->m_scanner->get_token_type() == integer)
                {
                    result->value = this->m_scanner->get_token_integer_value();
                    this->m_scanner->scan();
                    return result;
                }
            }
        }
    }
    delete result;
    return nullptr;
}

function_node::function_node()
{
    this->function_name = nullptr;
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

condition_node::~condition_node()
{
    if (this->variable_name != nullptr)
    {
        delete[] this->variable_name;
    }
}