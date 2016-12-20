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

struct symbols
{
    vector<function_symbols> functions;
};