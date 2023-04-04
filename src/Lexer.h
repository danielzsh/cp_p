#pragma once
#include <string>
#include <vector>
// TODO: consider refactoring to namespace vs. class?
class Lexer {
private:
    Lexer(std::string input);
    static char c;
    static char next_char();
    static std::string input;
    static int pos;
    static std::vector<std::string> keywords;
    static std::vector<std::string> types;
    static int line;
public:
    enum TokenType {
        tok_identifier,
        tok_int,
        tok_double,
        tok_eof,
        tok_string,
        tok_operator,
        tok_char,
        tok_keyword,
        tok_whitespace,
        tok_type
    };
    struct Token {
        TokenType type;
        std::string value;
    };
    static std::vector<Token> lex(std::string input);
    static Token get_tok();
};
std::ostream &operator<<(std::ostream &os, Lexer::Token const &token);
