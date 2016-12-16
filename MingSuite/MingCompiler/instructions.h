#pragma once

struct instruction
{
    instruction();
    virtual ~instruction();
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
    virtual ~label_instruction();
};

struct load_instruction : instruction
{
    virtual ~load_instruction();
    int destination_register;
    int location;
};

struct load_immediate_instruction : instruction
{
    virtual ~load_immediate_instruction();
    int destination_register;
    int value;
};

struct compare_instruction : instruction
{
    virtual ~compare_instruction();
    int destination_register;
    int operand1;
    int operand2;
};

struct store_instruction : instruction
{
    ~store_instruction();
    int location;
    int source_register;
};

struct branch_on_zero_instruction : instruction
{
    ~branch_on_zero_instruction();
    int operand;
    label_instruction* branchTo;
};

struct branch_instruction : instruction
{
    ~branch_instruction();
    label_instruction* branchTo;
};

struct plus_instruction : instruction
{
    virtual ~plus_instruction();
    int destination_register;
    int operand1;
    int operand2;
};

struct minus_instruction : instruction
{
    virtual ~minus_instruction();
    int destination_register;
    int operand1;
    int operand2;
};

struct call_instruction : instruction
{
    virtual ~call_instruction();
    label_instruction* target;
};

struct return_instruction : instruction
{
    virtual ~return_instruction();
};