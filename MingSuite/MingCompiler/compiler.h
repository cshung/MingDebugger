#pragma once

class compiler_impl;

class compiler
{
public:
    compiler(const char* source);
    void compile();
private:
    compiler_impl* impl;
};
