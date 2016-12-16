#include "virtual_machine.h"
#include <vector>
using namespace std;

class virtual_machine_impl
{
public:
    void run(instruction_sequence instructions, int entry_point);
private:
    void execute(instruction* instruction);
    vector<instruction*> code_memory;
    vector<int> data_memory;
    int ip;
    int sp;
    int r1;
    int r2;
    int r3;
    int r4;
};

virtual_machine::virtual_machine()
{
    this->impl = new virtual_machine_impl();
}

virtual_machine::~virtual_machine()
{
    delete this->impl;
}

void virtual_machine::run(instruction_sequence instructions, int entry_point)
{
    this->impl->run(instructions, entry_point);
}

void virtual_machine_impl::run(instruction_sequence instructions, int entry_point)
{
    data_memory.resize(255);
    instruction* cursor = instructions.head;
    while (cursor != nullptr)
    {
        this->code_memory.push_back(cursor);
        cursor = cursor->next;
    }
    sp = 254;
    data_memory[255] = -1;
    ip = entry_point;
    while (true)
    {
        instruction* current = code_memory[this->ip];
        this->execute(current);
    }
}

void virtual_machine_impl::execute(instruction* instruction)
{
    // TODO: Implementation
}