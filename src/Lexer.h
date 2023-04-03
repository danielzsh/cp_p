#pragma once
#include <string>
#include <vector>
namespace Lexer {
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
    std::ostream &operator<<(std::ostream &os, Token const &token);
    class Lexer {
    private:
        char c = ' ';
        char next_char();
        std::string input = "";
        int pos = -1;
        std::vector<std::string> keywords =
                {"input", "struct", "return", "define"};
        std::vector<std::string> types =
                {"int", "long"};
        int line = 1;
    public:
        Lexer(std::string input);
        std::vector<Token> lex();
        Token get_tok();
    };

} // Lexer

