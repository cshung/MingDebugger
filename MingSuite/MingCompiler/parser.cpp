#include "parser.h"

class parser_impl
{
public:
    parser_impl(scanner* scanner);
private:
    scanner* m_scanner;
};

parser::parser(scanner* scanner)
{
    this->impl = new parser_impl(scanner);
}

parser_impl::parser_impl(scanner* scanner) : m_scanner(scanner)
{
}