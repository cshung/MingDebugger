#pragma once

enum instruction_type
{
    label_instruction_type,
    load_instruction_type,
    load_immediate_instruction_type,
    compare_instruction_type,
    store_instruction_type,
    branch_on_zero_instruction_type,
    branch_instruction_type,
    plus_instruction_type,
    minus_instruction_type,
    call_instruction_type,
    return_instruction_type,
    push_instruction_type,
    pop_instruction_type,
    print_instruction_type,
    break_instruction_type,
};

struct instruction
{
    instruction();
    virtual ~instruction();
    virtual instruction_type get_instruction_type() = 0;

    virtual void print() = 0;

    instruction* prev;
    instruction* next;
};

struct instruction_sequence
{
    instruction_sequence();
    instruction* head;
    instruction* tail;
};

struct label_instruction : instruction
{
    label_instruction();
    virtual ~label_instruction();
    virtual instruction_type get_instruction_type();
    void assign_address(int address);
    virtual void print();

    static int label_counter;
    int label_id;
    int address;
};

struct load_instruction : instruction
{
    virtual ~load_instruction();
    virtual instruction_type get_instruction_type();

    virtual void print();
    int destination_register;
    int location;
};

struct load_immediate_instruction : instruction
{
    virtual ~load_immediate_instruction();
    virtual instruction_type get_instruction_type();

    virtual void print();
    int destination_register;
    int value;
};

struct compare_instruction : instruction
{
    virtual ~compare_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int destination_register;
    int operand1;
    int operand2;
};

struct store_instruction : instruction
{
    ~store_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int location;
    int source_register;
};

struct branch_on_zero_instruction : instruction
{
    ~branch_on_zero_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int operand;
    label_instruction* branchTo;
};

struct branch_instruction : instruction
{
    ~branch_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    label_instruction* branchTo;
};

struct plus_instruction : instruction
{
    virtual ~plus_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int destination_register;
    int operand1;
    int operand2;
};

struct minus_instruction : instruction
{
    virtual ~minus_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int destination_register;
    int operand1;
    int operand2;
};

struct call_instruction : instruction
{
    virtual ~call_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    label_instruction* target;
};

struct return_instruction : instruction
{
    virtual ~return_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();
};

struct push_instruction : instruction
{
    virtual ~push_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int offset;
};

struct pop_instruction : instruction
{
    virtual ~pop_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();

    int offset;
};

struct print_instruction : instruction
{
    virtual ~print_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();
};

struct break_instruction : instruction
{
    virtual ~break_instruction();
    virtual instruction_type get_instruction_type();
    virtual void print();
};
