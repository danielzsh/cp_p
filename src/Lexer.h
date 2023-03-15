#pragma once
#include <string>
#include <vector>
#include <map>
#include "Token.h"
namespace Lexer {
    class Lexer {
    private:
        char c = ' ';
        char nextchar();
        std::string input = "";
        int pos = -1;
        std::vector<std::string> keywords =
                {"input", "struct", "return"};
        std::map<std::string, int> kwd_index;
        int line = 1;
    public:
        Lexer(std::string input);
        std::vector<Token> lex();
        Token gettok();
    };

} // Lexer

