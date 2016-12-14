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
    expression_node* parse_expression();
    expression_node* parse_expression_suffix(expression_node* prefix);
    term_node* parse_term();
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
    result->function = parse_function();
    function_node* last_function = result->function;
    if (last_function != nullptr)
    {
        while (this->m_scanner->get_token_type() == function)
        {
            function_node* other = parse_function();
            if (other == nullptr)
            {
                if (result != nullptr)
                {
                    delete result;
                }
                return nullptr;
            }
            else
            {
                last_function->next_function = other;
                last_function = other;
            }
        }
        if (this->m_scanner->get_token_type() == eof)
        {
            return result;
        }
    }
    if (result != nullptr)
    {
        delete result;
    }
    return nullptr;
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
                                if (this->m_scanner->get_token_type() == right_brace)
                                {
                                    this->m_scanner->scan();
                                    return result;
                                }
                            }
                        }
                    }
                }
                else if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->m_scanner->scan();
                    if (this->m_scanner->get_token_type() == left_brace)
                    {
                        this->m_scanner->scan();
                        result->statement = this->parse_statement();
                        if (result->statement != nullptr)
                        {
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
                                    if (this->m_scanner->get_token_type() == left_brace)
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
    }
    else if (this->m_scanner->get_token_type() == _return)
    {
        return_statement_node* return_result = new return_statement_node();
        result = return_result;
        this->m_scanner->scan();
        return_result->value = this->parse_expression();
        if (return_result->value != nullptr)
        {
            if (this->m_scanner->get_token_type() == semi_colon)
            {
                this->m_scanner->scan();
                return result;
            }
        }
    }
    else if (this->m_scanner->get_token_type() == identifier)
    {
        call_statement_node* call_result = new call_statement_node();
        result = call_result;
        call_result->function_name = this->m_scanner->get_token_string();
        this->m_scanner->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            this->m_scanner->scan();
            call_result->argument = parse_expression();
            if (call_result->argument != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->m_scanner->scan();
                    if (this->m_scanner->get_token_type() == semi_colon)
                    {
                        this->m_scanner->scan();
                        return result;
                    }
                }
            }
        }
    }
    if (result != nullptr)
    {
        delete result;
    }
    return nullptr;
}

expression_node* parser_impl::parse_expression()
{
    expression_node* result = nullptr;
    term_node* term = this->parse_term();
    result = term;
    if (term != nullptr)
    {
        if (this->m_scanner->get_token_type() == _plus)
        {
            return this->parse_expression_suffix(term);
        }
        else if (this->m_scanner->get_token_type() == _minus)
        {
            return this->parse_expression_suffix(term);
        }
        else
        {
            return result;
        }
    }
    if (result != nullptr)
    {
        delete result;
    }
    return result;
}

expression_node* parser_impl::parse_expression_suffix(expression_node* prefix)
{
    expression_node* result = nullptr;
    if (this->m_scanner->get_token_type() == _plus)
    {
        plus_node* plus_result = new plus_node();
        result = plus_result;
        plus_result->left = prefix;
        this->m_scanner->scan();
        plus_result->right = this->parse_term();
        if (plus_result->right != nullptr)
        {
            if (this->m_scanner->get_token_type() == _plus)
            {
                return this->parse_expression_suffix(result);
            }
            else if (this->m_scanner->get_token_type() == _minus)
            {
                return this->parse_expression_suffix(result);
            }
            else
            {
                return result;
            }
        }
    }
    else if (this->m_scanner->get_token_type() == _minus)
    {
        minus_node* minus_result = new minus_node();
        result = minus_result;
        minus_result->left = prefix;
        this->m_scanner->scan();
        minus_result->right = this->parse_term();
        if (minus_result->right != nullptr)
        {
            if (this->m_scanner->get_token_type() == _plus)
            {
                return this->parse_expression_suffix(result);
            }
            else if (this->m_scanner->get_token_type() == _minus)
            {
                return this->parse_expression_suffix(result);
            }
            else
            {
                return result;
            }
        }
    }

    if (result != nullptr)
    {
        delete result;
    }
    return result;
}


term_node* parser_impl::parse_term()
{
    term_node* result = nullptr;
    if (this->m_scanner->get_token_type() == integer)
    {
        literal_node* literal_result = new literal_node();
        literal_result->value = this->m_scanner->get_token_integer_value();
        this->m_scanner->scan();
        return literal_result;
    }
    else if (this->m_scanner->get_token_type() == identifier)
    {
        char* identifier_name = this->m_scanner->get_token_string();
        this->m_scanner->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            call_node* call_result = new call_node();
            result = call_result;
            call_result->function_name = identifier_name;
            this->m_scanner->scan();
            call_result->argument = this->parse_expression();
            if (call_result->argument != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->m_scanner->scan();
                    return result;
                }
            }
        }
        else
        {
            identifier_node* identifier_result = new identifier_node();
            identifier_result->identifier_name = identifier_name;
            return identifier_result;
        }
    }

    if (result != nullptr)
    {
        delete result;
    }
    return result;
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
