#include "source_location.h"
#include <iostream>
using namespace std;

void source_location::show()
{
    cout << "(" << this->line << ":" << this->column << ")";
}

void source_span::show()
{
    this->begin.show();
    cout << " - ";
    this->end.show();
}