#pragma once

struct instruction
{
    instruction();
    virtual ~instruction();
    virtual bool is_label();
    instruction* prev;
    instruction* next;

    virtual void print() = 0;
};

struct instruction_sequence
{
    instruction_sequence();
    instruction* head;
    instruction* tail;
};

struct label_instruction : instruction
{
    static int label_counter;
    label_instruction();
    virtual ~label_instruction();
    int label_id;
    int address;

    virtual bool is_label();

    virtual void assign_address(int address);

    virtual void print();
};

struct load_instruction : instruction
{
    virtual ~load_instruction();
    int destination_register;
    int location;

    virtual void print();
};

struct load_immediate_instruction : instruction
{
    virtual ~load_immediate_instruction();
    int destination_register;
    int value;

    virtual void print();
};

struct compare_instruction : instruction
{
    virtual ~compare_instruction();
    int destination_register;
    int operand1;
    int operand2;

    virtual void print();
};

struct store_instruction : instruction
{
    ~store_instruction();
    int location;
    int source_register;

    virtual void print();
};

struct branch_on_zero_instruction : instruction
{
    ~branch_on_zero_instruction();
    int operand;
    label_instruction* branchTo;

    virtual void print();
};

struct branch_instruction : instruction
{
    ~branch_instruction();
    label_instruction* branchTo;

    virtual void print();
};

struct plus_instruction : instruction
{
    virtual ~plus_instruction();
    int destination_register;
    int operand1;
    int operand2;

    virtual void print();
};

struct minus_instruction : instruction
{
    virtual ~minus_instruction();
    int destination_register;
    int operand1;
    int operand2;

    virtual void print();
};

struct call_instruction : instruction
{
    virtual ~call_instruction();
    label_instruction* target;

    virtual void print();
};

struct return_instruction : instruction
{
    virtual ~return_instruction();

    virtual void print();
};

struct push_instruction : instruction
{
    virtual ~push_instruction();
    int offset;

    virtual void print();
};

struct pop_instruction : instruction
{
    virtual ~pop_instruction();
    int offset;

    virtual void print();
};


struct print_instruction : instruction
{
    virtual ~print_instruction();
    virtual void print();
};