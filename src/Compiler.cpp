#include "Compiler.h"
namespace Compiler {
    Compiler::Compiler(std::string input) {
        Lexer::Lexer lexer(input);
        tokens = lexer.lex();
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }
    std::string Compiler::compile() {
        int pos = 0;
        while (pos < tokens.size()) {
            Symbol cur = next_symbol(pos);
            if (cur.value == "define") { // #define statement
                header += compile_macro(pos);
            }
            else break;
            while (next_symbol(pos).value == "\n") pos++;
        }
        return header;
    }
    std::string Compiler::compile_macro(int& pos) {
        Symbol cur = next_symbol(pos);
        std::string statement = "#define " + (cur = next_symbol(pos += cur.tok_count)).value + " ";
        std::cout << next_symbol(pos).value << " " << next_symbol(pos).tok_count << std::endl;
        while (cur.value != "\n") {
            cur = next_symbol(pos += cur.tok_count);
            std::cout << pos << " " << cur.value << " " << cur.tok_count << std::endl;
            if (cur.type == sym_operator) statement += " " + cur.value + " ";
            else statement += cur.value;
        }
        std::cout << std::endl;
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
        if (tokens[pos].type == Lexer::tok_whitespace) {
            return Symbol(1, sym_whitespace, tokens[pos].value);
        }
        if (tokens[pos].type == Lexer::tok_identifier) { // function, function call, or assign
            if (pos + 1 >= tokens.size() || tokens[pos + 1].value != "(") return Symbol(1, sym_identifier, tokens[pos].value);
            std::string func = tokens[pos].value + "(";
            int tok_count = 2; // number of tokens
            pos += 2; // skip left paren
            SymbolType func_type;
            if (tokens[pos].value == ")") { // could be function declaration or function call
                if (pos + 1 >= tokens.size() || tokens[pos + 1].value != ":") func_type = sym_func_call;
                return Symbol(3, func_type, func + ")");
            }
            if (tokens[pos].type == Lexer::tok_identifier) { // function call (e.g. SQUARE(x))
                func_type = sym_func_call;
            } else func_type = sym_func; // function signature (e.g. square(int x))
            while (tokens[pos].value != ")") {
                tok_count++;
                func += tokens[pos].value;
                pos++;
            }
            return Symbol(tok_count + 1, func_type, func + ")");
        }
        if (tokens[pos].type == Lexer::tok_char) {
            return Symbol(1, sym_char, tokens[pos].value);
        }
        if (tokens[pos].type == Lexer::tok_operator) {
            return Symbol(1, sym_operator, tokens[pos].value);
        }
    }
}