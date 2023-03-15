#include <iostream>
#include <string>
enum TokenType {
    tok_identifier,
    tok_int,
    tok_double,
    tok_eof,
    tok_string,
    tok_char,
    tok_keyword,
    tok_whitespace,
    tok_symbol
};
class Token {
public:
    TokenType type;
    std::string value;
    Token() {}
    Token(TokenType type, std::string value) : type(type), value(value) {}
};
std::ostream &operator<<(std::ostream &os, Token const &token);
