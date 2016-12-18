#pragma once

enum token_type
{
    semi_colon,
    left_brace,
    left_bracket,
    right_brace,
    right_bracket,
    equals,
    function,
    _if,
    _else,
    _return,
    identifier,
    integer,
    eof,
    _plus,
    _minus,
    error,
};

class scanner_impl;

class scanner
{
public:
    scanner(const char* source_file);
    ~scanner();
    void scan();
    token_type get_token_type();
    const char* get_token_begin();
    const char* get_token_end();
    int get_token_begin_line();
    int get_token_begin_column();
    int get_token_end_line();
    int get_token_end_column();
    char* get_token_string();
    int get_token_integer_value();
private:
    scanner_impl* impl;
};