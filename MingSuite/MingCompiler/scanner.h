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
    void scan();
    token_type get_token_type();
private:
    scanner_impl* impl;
};