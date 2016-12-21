#pragma once

struct source_location
{
    int line;
    int column;
    void show();
};

struct source_span
{
    source_location begin;
    source_location end;
    void show();
};
