#pragma once
#include "Lexer.h"
#include <iostream>
namespace Compiler {
    enum SymbolType {
        sym_type, // int, long long, etc.
        sym_keyword,
        sym_symbol, // ->, ()
        sym_whitespace,
    };
    class Symbol {
    public:
        int tok_count;
        SymbolType type;
        std::string value;
        Symbol() {}
        Symbol(int tok_count, SymbolType type, std::string value)
        : tok_count(tok_count), type(type), value(value) {}
    };
    class Compiler {
    private:
        std::vector<Lexer::Token> tokens;
        std::string header = "#include <bits/stdc++.h>\n";
        std::string body;
        std::string compile_main(int pos);
        std::string compile_macro(int& pos);
        Symbol next_symbol(int pos);
    public:
        Compiler(std::string input);
        std::string compile();
    };
}