#pragma once

enum token_type
{
    function,
    identifier,
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