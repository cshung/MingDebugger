#pragma once

class compiler_impl;

class compiler
{
public:
    compiler(const char* source_file);
    ~compiler();
    void compile();
private:
    compiler_impl* impl;
};
