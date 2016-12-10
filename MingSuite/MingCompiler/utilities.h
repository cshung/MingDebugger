#pragma once

bool is_file_exists(const char* filename);

bool is_ends_with(const char* source, const char* pattern);

char* read_all_text(const char* filename);