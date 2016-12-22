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

        int address = 0;
        instruction* cursor = binaries.instructions.head;
        while (cursor != nullptr)
        {
            cout << address << ": ";
            cursor->print();
            cursor = cursor->next;
            address++;
        }

        // TODO: Save the binaries
        virtual_machine vm;
        debugger* debugger = vm.debug(binaries.instructions, &binaries.symbols, binaries.entry_point);

        // Setting a breakpoint at the entry point of the fib function
        breakpoint* bp = debugger->create_address_breakpoint(0);
        debugger->resume();
        // Reach the breakpoint - what is the argument?
        cout << debugger->get_context().r1 << endl;
        debugger->resume();
        // Reach the breakpoint again - what is the argument?
        cout << debugger->get_context().r1 << endl;
        // Removing the breakpoint
        bp->remove();
        cout << debugger->get_context().ip << endl;
        // Step over the prolog
        debugger->step_instruction();
        debugger->step_instruction();
        // Read the argument through raw memory access
        int argument_address = debugger->get_context().sp + 1;
        cout << debugger->read_memory(argument_address) << endl;

        // Starting source debugging
        cout << "=================================" << endl;

        // Starting source debugging
        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        debugger->step_into();
        
        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        debugger->step_into();

        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        debugger->step_into();

        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        debugger->step_out();

        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        debugger->step_over();

        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();
        
        breakpoint* source_breakpoint = debugger->create_source_location_breakpoint(2, 1);
        debugger->resume();

        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        source_breakpoint->remove();

        debugger->resume();
        debugger->get_source_span().show(); cout << endl;
        debugger->stack_walk();

        // Now we run to the end!
    }
}
