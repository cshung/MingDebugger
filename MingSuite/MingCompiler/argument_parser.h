#pragma once

struct arguments
{
    arguments();
    const char* error;
    const char* filename;
};

arguments check_arguments(int argc, char** argv);
