#include "compiler.h"

#include "scanner.h"

#include <iostream>
using namespace std;

class compiler_impl
{
public:
    compiler_impl(const char* source_file);
    ~compiler_impl();
    void compile();
private:
    const char* source_file;
};

compiler::compiler(const char* source_file)
{
    this->impl = new compiler_impl(source_file);
}

compiler::~compiler()
{
    delete this->impl;
}

void compiler::compile()
{
    this->impl->compile();
}

compiler_impl::compiler_impl(const char* source_file)
{
    this->source_file = source_file;
}

compiler_impl::~compiler_impl()
{

}

void compiler_impl::compile()
{
    scanner s(this->source_file);
    while (true)
    {
        s.scan();
        switch (s.get_token_type())
        {
        case semi_colon:
            cout << ';';
            break;
        case left_brace:
            cout << '{';
            break;
        case left_bracket:
            cout << '(';
            break;
        case right_brace:
            cout << '}';
            break;
        case right_bracket:
            cout << ')';
            break;
        case equals:
            cout << '=';
            break;
        case function:
            cout << "function";
            break;
        case _if:
            cout << "if";
            break;
        case _else:
            cout << "else";
            break;
        case _return:
            cout << "return";
            break;
        case identifier:
            cout << "identifier";
            break;
        case integer:
            cout << "integer";
            break;
        case eof:
            cout << "eof";
            break;
        case _plus:
            cout << '+';
            break;
        case _minus:
            cout << '-';
            break;
        case error:
            cout << "ERROR!";
            break;
        }
        cout << endl;
        for (const char* p = s.get_token_begin(); p < s.get_token_end(); p++)
        {
            cout << *p;
        }
        cout << endl;

        if (s.get_token_type() == eof)
        {
            return;
        }
    }
}
