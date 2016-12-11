#include "scanner.h"
#include "utilities.h"
#include <iostream>
using namespace std;

scanner::scanner(const char* source_file)
{
    this->source_text = read_all_text(source_file);
}