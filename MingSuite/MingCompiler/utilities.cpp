#include "utilities.h"
#include <fstream>
using namespace std;

bool is_file_exists(const char* filename)
{
    ifstream f(filename);
    return f.good();
}

bool is_ends_with(const char* source, const char* pattern)
{
    int source_length = strlen(source);
    int pattern_length = strlen(pattern);
    if (pattern_length > source_length)
    {
        return false;
    }

    for (int i = 0; i < pattern_length; i++)
    {
        if (source[source_length - pattern_length + i] != pattern[i])
        {
            return false;
        }
    }

    return true;
}