#include "Lexer.h"
#include <iostream>
namespace Lexer {
    std::ostream &operator<<(std::ostream &os, Token const &token) {
        std::string value = token.value;
        if (value == "\n") value = "\\n";
        if (value == "\t") value = "\\t"; 
        return os << token.type << " " << value;
    }
    Lexer::Lexer(std::string input) {
        this->input = input;
    }
    std::vector<Token> Lexer::lex() {
        Token curtok;
        std::vector<Token> res;
        while ((curtok = get_tok()).type != tok_eof) res.push_back(curtok);
        return res;
    }
    char Lexer::next_char() {
        if (++pos >= input.size()) return EOF;
        return input[pos];
    }
    Token Lexer::get_tok() {
        while (std::isspace(c)) {
            if (c == '\n') {
                return c = next_char(), Token(tok_whitespace, "\n");
            } else if (c == '\t') {
                return c = next_char(), Token(tok_whitespace, "\t");
            }
            c = next_char();
        }
        if (std::isalpha(c)) {
            std::string str(1, c);
            while (std::isalnum(c = next_char())) {
                str += c;
            }
            for (int i = 0; i < types.size(); i++) {
                if (str == types[i]) return Token(tok_type, str);
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
                c = next_char();
            } while (std::isdigit(c) || c == '.');
            return Token(type, str);
        }
        if (c == '\'') {
            std::string str = "";
            while ((c = next_char()) != '\'') {
                str += c;
            }
            c = next_char();
            return Token(tok_string, str);
        }
        if (c == '#') {
            do {
                c = next_char();
            } while (c != EOF && c != '\n' && c != '\r');
            if (c != EOF) {
                return get_tok();
            }
        }
        if (c == EOF) return Token(tok_eof, "");
        char thischar = c;
        c = next_char();
        return Token(tok_char, std::string(1, thischar));
    }
} // Lexer