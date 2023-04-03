#pragma once
#include "Lexer.h"
#include <iostream>
namespace Compiler {
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
        int pos;
        int tok_count;
        SymbolType type;
        std::string value;
    };
    std::ostream &operator<<(std::ostream &os, Symbol const& sym);
    class Compiler {
    private:
        std::vector<Lexer::Token> tokens;
        std::string header = "#include <bits/stdc++.h>\nusing namespace std;\n";
        std::string body;
        std::string compile_main(int pos);
        std::string compile_macro(Symbol& sym);
        std::string compile_struct(int& pos);
        std::string compile_block(int& pos, int tabs);
        Symbol advance(Symbol& sym);
        Symbol peek(Symbol sym);
        Symbol next_symbol(int pos);
    public:
        Compiler(std::string input);
        std::string compile();
    };
}