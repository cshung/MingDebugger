#pragma once

#include "scanner.h"
#include "nodes.h"

class parser
{
public:
    parser(scanner* scanner);
    program_node* parse();
private:
    parser_impl* impl;
};