#include "argument_parser.h"
#include "utilities.h"

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
    else if (!is_ends_with(argv[1], ".source"))
    {
        result.error = "Source file must have extension .source";
    }
    else if (!is_file_exists(argv[1]))
    {
        result.error = "source file does not exists";
    }
    else
    {
        result.filename = argv[1];
    }

    return result;
}

