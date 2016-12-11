#pragma once

#include "scanner.h"

class parser_impl;

class parser
{
public:
    parser(scanner* scanner);
private:
    parser_impl* impl;
};