#include "utilities.h"
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

bool is_file_exists(const char* filename)
{
    ifstream f(filename);
    return f.good();
}

bool is_begins_with(const char* source, const char* pattern)
{
    size_t source_length = strlen(source);
    size_t pattern_length = strlen(pattern);
    if (pattern_length > source_length)
    {
        return false;
    }

    for (size_t  i = 0; i < pattern_length; i++)
    {
        if (source[i] != pattern[i])
        {
            return false;
        }
    }

    return true;
}

bool is_ends_with(const char* source, const char* pattern)
{
    size_t  source_length = strlen(source);
    size_t  pattern_length = strlen(pattern);
    if (pattern_length > source_length)
    {
        return false;
    }

    for (size_t  i = 0; i < pattern_length; i++)
    {
        if (source[source_length - pattern_length + i] != pattern[i])
        {
            return false;
        }
    }

    return true;
}

char* read_all_text(const char* filename)
{
    vector<char> buffer;
    ifstream f(filename);
    while (true)
    {
        char c = f.get();
        if (f.eof())
        {
            break;
        }
        buffer.push_back(c);
    }
    char* result = new char[buffer.size() + 1];
    for (size_t i = 0; i < buffer.size(); i++)
    {
        result[i] = buffer[i];
    }
    result[buffer.size()] = '\0';
    return result;
}
