#include "Lexer.h"
#include <iostream>
std::ostream &operator<<(std::ostream &os, Token const &token) {
    std::string value = token.value;
    if (value == "\n") value = "\\n";
    if (value == "\t") value = "\\t"; 
    return os << token.type << " " << value;
}
namespace Lexer {
    Lexer::Lexer(std::string input) {
        this->input = input;
        for (int i = 0; i < keywords.size(); i++) {
            kwd_index[keywords[i]] = i;
        }
    }
    std::vector<Token> Lexer::lex() {
        Token curtok;
        std::vector<Token> res;
        while ((curtok = gettok()).type != tok_eof) res.push_back(curtok);
        return res;
    }
    char Lexer::nextchar() {
        if (++pos >= input.size()) return EOF;
        return input[pos];
    }
    Token Lexer::gettok() {
        while (std::isspace(c)) {
            if (c == '\n') {
                return c = nextchar(), Token(tok_whitespace, "\n");
            } else if (c == '\t') {
                return c = nextchar(), Token(tok_whitespace, "\t");
            }
            c = nextchar();
        }
        if (std::isalpha(c)) {
            std::string str(1, c);
            while (std::isalnum(c = nextchar())) {
                str += c;
            }
            for (int i = 0; i < keywords.size(); i++) {
                if (str == keywords[i]) return Token(tok_keyword, str);
            }
            return Token(tok_identifier, str);
        }
        if (std::isdigit(c) || c == '.') {
            std::string str;
            TokenType type = tok_int;
            do {
                if (c == '.') type = tok_double;
                str += c;
                c = nextchar();
            } while (std::isdigit(c) || c == '.');
            return Token(type, str);
        }
        if (c == '\'') {
            std::string str = "";
            while ((c = nextchar()) != '\'') {
                str += c;
            }
            c = nextchar();
            return Token(tok_string, str);
        }
        if (c == '#') {
            do {
                c = nextchar();
            } while (c != EOF && c != '\n' && c != '\r');
            if (c != EOF) {
                return gettok();
            }
        }
        if (c == EOF) return Token(tok_eof, "");
        char thischar = c;
        c = nextchar();
        if (thischar == '-' && c == '>') return c = nextchar(), Token(tok_symbol, "->");
        return Token(tok_char, std::string(1, thischar));
    }
} // Lexer