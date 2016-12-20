#pragma once

#include <string>
#include <vector>
using namespace std;


struct local_symbols
{
    string local_name;
    int address;
};

struct function_symbols
{
    string function_name;
    int entry_point;
    int after_exit;
    vector<local_symbols> locals;
};

struct statement
{
    int start_line;
    int start_column;
    int end_line;
    int end_column;
    int start_address;
    int end_address;
};

struct symbols
{
    vector<function_symbols> functions;
    vector<statement> statements;
};