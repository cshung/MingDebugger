#include "parser.h"

class parser_impl
{
public:
    parser_impl(scanner* scanner);
    program_node* parse();
private:
    void scan();
    scanner* m_scanner;
    int m_last_token_end_line;
    int m_last_token_end_column;

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
    this->scan();
    return this->parse_program();
}

void parser_impl::scan()
{
    this->m_last_token_end_line = this->m_scanner->get_token_end_line();
    this->m_last_token_end_column = this->m_scanner->get_token_end_column();
    this->m_scanner->scan();
}

program_node* parser_impl::parse_program()
{
    program_node* result = new program_node();
    result->source_span.begin.line = this->m_scanner->get_token_begin_line();
    result->source_span.begin.column = this->m_scanner->get_token_begin_column();
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
            result->source_span.end.line = this->m_last_token_end_line;
            result->source_span.end.column = this->m_last_token_end_column;
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
    result->source_span.begin.line = this->m_scanner->get_token_begin_line();
    result->source_span.begin.column = this->m_scanner->get_token_begin_column();
    if (this->m_scanner->get_token_type() == function)
    {
        this->scan();
        if (this->m_scanner->get_token_type() == identifier)
        {
            result->function_name = this->m_scanner->get_token_string();
            this->scan();
            if (this->m_scanner->get_token_type() == left_bracket)
            {
                this->scan();
                if (this->m_scanner->get_token_type() == identifier)
                {
                    result->argument_name = this->m_scanner->get_token_string();
                    this->scan();
                    if (this->m_scanner->get_token_type() == right_bracket)
                    {
                        this->scan();
                        if (this->m_scanner->get_token_type() == left_brace)
                        {
                            this->scan();
                            result->statement = this->parse_statement();
                            if (result->statement != nullptr)
                            {
                                if (this->m_scanner->get_token_type() == right_brace)
                                {
                                    this->scan();
                                    result->source_span.end.line = this->m_last_token_end_line;
                                    result->source_span.end.column = this->m_last_token_end_column;
                                    return result;
                                }
                            }
                        }
                    }
                }
                else if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->scan();
                    if (this->m_scanner->get_token_type() == left_brace)
                    {
                        this->scan();
                        result->statement = this->parse_statement();
                        if (result->statement != nullptr)
                        {
                            if (this->m_scanner->get_token_type() == right_brace)
                            {
                                this->scan();
                                result->source_span.end.line = this->m_last_token_end_line;
                                result->source_span.end.column = this->m_last_token_end_column;
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
        result->source_span.begin.line = this->m_scanner->get_token_begin_line();
        result->source_span.begin.column = this->m_scanner->get_token_begin_column();
        this->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            this->scan();
            if_result->condition = parse_condition();
            if (if_result->condition != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->scan();
                    if (this->m_scanner->get_token_type() == left_brace)
                    {
                        this->scan();
                        if_result->true_statement = parse_statement();
                        if (if_result->true_statement != nullptr)
                        {
                            if (this->m_scanner->get_token_type() == right_brace)
                            {
                                this->scan();
                                if (this->m_scanner->get_token_type() == _else)
                                {
                                    this->scan();
                                    if (this->m_scanner->get_token_type() == left_brace)
                                    {
                                        this->scan();
                                        if_result->false_statement = parse_statement();
                                        if (if_result->false_statement != nullptr)
                                        {
                                            if (this->m_scanner->get_token_type() == right_brace)
                                            {
                                                this->scan();
                                                result->source_span.end.line = this->m_last_token_end_line;
                                                result->source_span.end.column = this->m_last_token_end_column;
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
        result->source_span.begin.line = this->m_scanner->get_token_begin_line();
        result->source_span.begin.column = this->m_scanner->get_token_begin_column();
        this->scan();
        return_result->value = this->parse_expression();
        if (return_result->value != nullptr)
        {
            if (this->m_scanner->get_token_type() == semi_colon)
            {
                this->scan();
                result->source_span.end.line = this->m_last_token_end_line;
                result->source_span.end.column = this->m_last_token_end_column;
                return result;
            }
        }
    }
    else if (this->m_scanner->get_token_type() == identifier)
    {
        call_statement_node* call_result = new call_statement_node();
        result = call_result;
        result->source_span.begin.line = this->m_scanner->get_token_begin_line();
        result->source_span.begin.column = this->m_scanner->get_token_begin_column();
        call_result->function_name = this->m_scanner->get_token_string();
        this->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            this->scan();
            call_result->argument = parse_expression();
            if (call_result->argument != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->scan();
                    if (this->m_scanner->get_token_type() == semi_colon)
                    {
                        this->scan();
                        result->source_span.end.line = this->m_last_token_end_line;
                        result->source_span.end.column = this->m_last_token_end_column;
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
        result->source_span.begin.line = prefix->source_span.begin.line;
        result->source_span.begin.column = prefix->source_span.begin.column;
        plus_result->left = prefix;
        this->scan();
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
                result->source_span.end.line = this->m_last_token_end_line;
                result->source_span.end.column = this->m_last_token_end_column;
                return result;
            }
        }
    }
    else if (this->m_scanner->get_token_type() == _minus)
    {
        minus_node* minus_result = new minus_node();
        result = minus_result;
        result->source_span.begin.line = prefix->source_span.begin.line;
        result->source_span.begin.column = prefix->source_span.begin.column;
        minus_result->left = prefix;
        this->scan();
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
                result->source_span.end.line = this->m_last_token_end_line;
                result->source_span.end.column = this->m_last_token_end_column;
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
        literal_result->source_span.begin.line = this->m_scanner->get_token_begin_line();
        literal_result->source_span.begin.column = this->m_scanner->get_token_begin_column();
        literal_result->value = this->m_scanner->get_token_integer_value();
        this->scan();
        literal_result->source_span.end.line = this->m_last_token_end_line;
        literal_result->source_span.end.column = this->m_last_token_end_column;
        return literal_result;
    }
    else if (this->m_scanner->get_token_type() == identifier)
    {
        int begin_line = this->m_scanner->get_token_begin_line();
        int begin_column = this->m_scanner->get_token_begin_column();
        char* identifier_name = this->m_scanner->get_token_string();
        this->scan();
        if (this->m_scanner->get_token_type() == left_bracket)
        {
            call_node* call_result = new call_node();
            result = call_result;
            result->source_span.begin.line = begin_line;
            result->source_span.begin.column = begin_column;
            call_result->function_name = identifier_name;
            this->scan();
            call_result->argument = this->parse_expression();
            if (call_result->argument != nullptr)
            {
                if (this->m_scanner->get_token_type() == right_bracket)
                {
                    this->scan();
                    result->source_span.end.line = this->m_last_token_end_line;
                    result->source_span.end.column = this->m_last_token_end_column;
                    return result;
                }
            }
        }
        else
        {
            identifier_node* identifier_result = new identifier_node();
            identifier_result->source_span.begin.line = begin_line;
            identifier_result->source_span.begin.column = begin_column;
            identifier_result->identifier_name = identifier_name;
            identifier_result->source_span.end.line = this->m_last_token_end_line;
            identifier_result->source_span.end.column = this->m_last_token_end_column;
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
    result->source_span.begin.line = this->m_scanner->get_token_begin_line();
    result->source_span.begin.column = this->m_scanner->get_token_begin_column();
    if (this->m_scanner->get_token_type() == identifier)
    {
        result->variable_name = this->m_scanner->get_token_string();
        this->scan();
        if (this->m_scanner->get_token_type() == equals)
        {
            this->scan();
            if (this->m_scanner->get_token_type() == equals)
            {
                this->scan();
                if (this->m_scanner->get_token_type() == integer)
                {
                    result->value = this->m_scanner->get_token_integer_value();
                    this->scan();
                    result->source_span.end.line = this->m_last_token_end_line;
                    result->source_span.end.column = this->m_last_token_end_column;
                    return result;
                }
            }
        }
    }
    delete result;
    return nullptr;
}
