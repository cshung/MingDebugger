#pragma once

#include <string>
#include <vector>
using namespace std;

struct function_symbols
{
    string function_name;
    int entry_point;
    int after_prolog;
    int after_exit;
};

struct symbols
{
    vector<function_symbols> functions;
};