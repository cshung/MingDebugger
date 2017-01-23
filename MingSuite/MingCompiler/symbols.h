#pragma once

#include "source_location.h"
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
    vector<struct local_symbols> locals;
};

struct statement_symbol
{
    struct source_span source_span;
    int start_address;
};

struct symbols
{
    vector<struct function_symbol> functions;
    vector<struct statement_symbol> statements;
};
