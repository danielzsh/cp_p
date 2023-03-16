#include "Compiler.h"
namespace Compiler {
    Compiler::Compiler(std::string input) {
        Lexer::Lexer lexer(input);
        tokens = lexer.lex();
    }
    std::string Compiler::compile() {
        int pos = 0;
        while (pos < tokens.size()) {
            Symbol cur = next_symbol(pos);
            if (next_symbol(pos + cur.tok_count).value == "->") { // #define statement
                header += compile_macro(pos);
                std::cout << pos << "\n";
                return header;
            }
            while (next_symbol(pos).value == "\n") pos++;
        }
    }
    std::string Compiler::compile_macro(int& pos) {
        Symbol cur = next_symbol(pos);
        std::string statement = "#define " + cur.value;
        cur = next_symbol(pos += cur.tok_count); // ->
        while (cur.value != "\n") {
            cur = next_symbol(pos += cur.tok_count);
            statement += " " + cur.value;
        }
        return statement;
    }
    Symbol Compiler::next_symbol(int pos) {
        if (pos >= tokens.size()) throw std::string("Expected token after " + tokens.back().value);
        if (tokens[pos].type == Lexer::tok_type) {
            if (tokens[pos].value == "long") { // expect long long
                if (++pos >= tokens.size() || tokens[pos].value != "long") {
                    throw std::string("token 'long' must be extended to 'long long'.");
                }
                return Symbol(2, sym_type, "long long");
            }
            return Symbol(1, sym_type, tokens[pos].value); 
        }
        if (tokens[pos].type == Lexer::tok_keyword) {
            return Symbol(1, sym_keyword, tokens[pos].value);
        }
        if (tokens[pos].type == Lexer::tok_symbol) {
            return Symbol(1, sym_symbol, tokens[pos].value);
        }
        if (tokens[pos].type == Lexer::tok_whitespace) {
            return Symbol(1, sym_whitespace, tokens[pos].value);
        }
    }
}