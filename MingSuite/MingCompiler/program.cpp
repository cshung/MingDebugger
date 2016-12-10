#include <iostream>

#include "argument_parser.h"
#include "compiler.h"

using namespace std;

int main(int argc, char** argv)
{
    arguments args = check_arguments(argc, argv);
    if (args.error)
    {
        cout << args.error << endl;
        return 0;
    }
    compiler c(args.filename);
    c.compile();
    return 0;
}