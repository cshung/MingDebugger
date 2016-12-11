#include "scanner.h"
#include "utilities.h"

#include <iostream>
using namespace std;

class scanner_impl
{
public:
    scanner_impl(const char* source_file);
    ~scanner_impl();
    void scan();
    token_type get_token_type();
    const char* get_token_begin();
    const char* get_token_end();
    char* get_token_string();
    int get_token_integer_value();
private:
    const char* source_text;
    const char* p;
    token_type m_token_type;
    const char* m_token_begin;
    const char* m_token_end;
};

scanner::scanner(const char* source_file)
{
    this->impl = new scanner_impl(source_file);
}

scanner::~scanner()
{
    delete this->impl;
}

void scanner::scan()
{
    this->impl->scan();
}

token_type scanner::get_token_type()
{
    return impl->get_token_type();
}

const char* scanner::get_token_begin()
{
    return this->impl->get_token_begin();
}

const char* scanner::get_token_end()
{
    return this->impl->get_token_end();
}

char* scanner::get_token_string()
{
    return this->impl->get_token_string();
}

int scanner::get_token_integer_value()
{
    return this->impl->get_token_integer_value();
}

scanner_impl::scanner_impl(const char* source_file)
{
    this->source_text = read_all_text(source_file);
    this->p = this->source_text;
}

scanner_impl::~scanner_impl()
{
    delete[] this->source_text;
}

void scanner_impl::scan()
{
    // Step 1: Skip away any whitespace, not interesting at all
    while (*p == ' ' || *p == '\r' || *p == '\n')
    {
        p++;
    }
    this->m_token_begin = p;
    char c = *p;
    switch (c)
    {
    case ';':
        this->m_token_type = semi_colon;
        p++;
        break;
    case '{':
        this->m_token_type = left_brace;
        p++;
        break;
    case '}':
        this->m_token_type = right_brace;
        p++;
        break;
    case '(':
        this->m_token_type = left_bracket;
        p++;
        break;
    case ')':
        this->m_token_type = right_bracket;
        p++;
        break;
    case '=':
        this->m_token_type = equals;
        p++;
        break;
    case '+':
        this->m_token_type = _plus;
        p++;
        break;
    case '-':
        this->m_token_type = _minus;
        p++;
        break;
    case '\0':
        this->m_token_type = eof;
        break;
    default:
        if (is_begins_with(p, "function"))
        {
            p = p + 8;
            c = *p;
            if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')
            {
                this->m_token_type = function;
                break;
            }
        }
        else if (is_begins_with(p, "if"))
        {
            p = p + 2;
            c = *p;
            if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')
            {
                this->m_token_type = _if;
                break;
            }
        }
        else if (is_begins_with(p, "else"))
        {
            p = p + 4;
            c = *p;
            if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')
            {
                this->m_token_type = _else;
                break;
            }
        }
        else if (is_begins_with(p, "return"))
        {
            p = p + 6;
            c = *p;
            if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\0')
            {
                this->m_token_type = _return;
                break;
            }
        }
        else if ('a' <= *p && *p <= 'z')
        {
            while ('a' <= *p && *p <= 'z')
            {
                p++;
            }
            this->m_token_type = identifier;
        }
        else if ('0' <= *p && *p <= '9')
        {
            while ('0' <= *p && *p <= '9')
            {
                p++;
            }
            this->m_token_type = integer;
        }
        else
        {
            cout << p;
            this->m_token_type = error;
        }
    }
    this->m_token_end = p;
}

token_type scanner_impl::get_token_type()
{
    return this->m_token_type;
}

const char* scanner_impl::get_token_begin()
{
    return this->m_token_begin;
}

const char* scanner_impl::get_token_end()
{
    return this->m_token_end;
}

char* scanner_impl::get_token_string()
{
    int length = this->get_token_end() - this->get_token_begin();
    char* result = new char[length + 1];
    char* output = result;
    for (const char* p = this->get_token_begin(); p < this->get_token_end(); p++, output++)
    {
        *output = *p;
    }
    *output = '\0';
    return result;
}

int scanner_impl::get_token_integer_value()
{
    int result = 0;
    for (const char* p = this->get_token_begin(); p < this->get_token_end(); p++)
    {
        result *= 10;
        result += (*p - '0');
    }
    return result;
}
