#include <iostream>

#include "argument_parser.h"

using namespace std;

int main(int argc, char** argv)
{
    arguments argument = check_arguments(argc, argv);
    if (argument.error)
    {
        cout << argument.error << endl;
        return 0;
    }

    return 0;
}