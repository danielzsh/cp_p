#pragma once
#include "Lexer.h"
#include <iostream>
class Compiler {
public:
    enum SymbolType {
        sym_type, // int, long long, etc.
        sym_keyword,
        sym_whitespace,
        sym_identifier,
        sym_func,
        sym_func_call,
        sym_char,
        sym_operator,
        sym_eof,
        sym_start
    };
    struct Symbol {
        int pos; // ending position of symbol
        SymbolType type;
        std::string value;
        Symbol& advance();
        Symbol peek();
    };
    static std::string compile(std::string input);
private:
    Compiler();
    static std::vector<Lexer::Token> tokens;
    static std::string header;
    static std::string body;
    static std::string compile_main(int pos);
    static std::string compile_macro(Symbol& sym);
    static std::string compile_struct(Symbol& sym);
    static std::string compile_func(Symbol& sym);
    static std::string compile_block(Symbol& sym, int tabs);
    static std::string compile_decl(Symbol& sym);
    static std::string copy_line(Symbol& sym);
    static Symbol next_symbol(int pos);
    static bool intToLL; // whether int was defined as long long

    friend class Symbol;
};
std::ostream &operator<<(std::ostream &os, Compiler::Symbol const& sym);
