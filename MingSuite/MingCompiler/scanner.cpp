#include "scanner.h"
#include "utilities.h"

class scanner_impl
{
public:
    scanner_impl(const char* source_file);
    void scan();
    token_type get_token_type();
private:
    const char* source_text;
    const char* p;
    token_type m_token_type;
};

scanner::scanner(const char* source_file)
{
    this->impl = new scanner_impl(source_file);
}

void scanner::scan()
{
    this->impl->scan();
}

token_type scanner::get_token_type()
{
    return impl->get_token_type();
}

scanner_impl::scanner_impl(const char* source_file)
{
    this->source_text = read_all_text(source_file);
    this->p = this->source_text;
}

void scanner_impl::scan()
{
    // Step 1: Skip away any whitespace, not interesting at all
    while (*p == ' ')
    {
        p++;
    }
    if (is_begins_with(p, "function "))
    {
        this->m_token_type = function;
        p = p + 8;
    }
    else
    {
        this->m_token_type = identifier;
    }
}

token_type scanner_impl::get_token_type()
{
    return this->m_token_type;
}