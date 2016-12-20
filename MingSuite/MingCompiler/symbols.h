#pragma once

#include <string>
#include <vector>
using namespace std;


struct local_symbols
{
    string local_name;
    int address;
};

struct function_symbol
{
    string function_name;
    int entry_point;
    int after_exit;
    vector<local_symbols> locals;
};

struct statement_symbol
{
    int begin_line;
    int begin_column;
    int end_line;
    int end_column;
    int start_address;
};

struct symbols
{
    vector<function_symbol> functions;
    vector<statement_symbol> statements;
};