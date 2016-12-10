#include "argument_parser.h"
#include <fstream>

using namespace std;

arguments::arguments()
{
    this->error = nullptr;
    this->filename = nullptr;
}

arguments check_arguments(int argc, char** argv)
{
    arguments result;
    if (argc != 2)
    {
        result.error = "MingCompiler program.source";
    }
    else
    {
        ifstream f(argv[1]);
        if (f.good())
        {
            result.filename = argv[1];
        }
        else
        {
            result.error = "source file does not exists";
        }
    }

    return result;
}
