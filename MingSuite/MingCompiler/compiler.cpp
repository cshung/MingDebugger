#include "compiler.h"

#include "scanner.h"
#include "parser.h"
#include "code_generator.h"

// TODO: Remove me!
#include "virtual_machine.h"

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
    parser p(&s);
    program_node* program = p.parse();
    if (program != nullptr)
    {
        code_generator c;
        code_generation_outputs binaries = c.generate_code(program);
        // TODO: Save the binaries
        virtual_machine vm;
        debugger* debugger = vm.debug(binaries.instructions, binaries.entry_point);
        debugger->resume();
    }
}
