#pragma once

class compiler_impl;

class compiler
{
public:
    compiler(const char* source_file);
    void compile();
private:
    compiler_impl* impl;
};
