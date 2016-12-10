#include "compiler.h"
#include <iostream>
using namespace std;

class compiler_impl
{
public:
    compiler_impl(const char* source);
    void compile();
private:
    const char* source;
};

compiler::compiler(const char* source)
{
    this->impl = new compiler_impl(source);
}

void compiler::compile()
{
    this->impl->compile();
}

compiler_impl::compiler_impl(const char* source)
{
    this->source = source;
}

void compiler_impl::compile()
{
    cout << "do something with " << this->source << endl;
}
