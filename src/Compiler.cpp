#include "Compiler.h"
#include <assert.h>
namespace Compiler {
    std::ostream &operator<<(std::ostream &os, Symbol const& sym) {
        std::string value = sym.value;
        if (value == "\n") value = "\\n";
        if (value == "\t") value = "\\t"; 
        return os << sym.pos << " " << sym.tok_count << " " << sym.type << " " << value;
    }
    Compiler::Compiler(std::string input) {
        Lexer::Lexer lexer(input);
        tokens = lexer.lex();
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }
    std::string Compiler::compile() {
        Symbol cur_sym = {-1, 0, sym_start, ""};
        while (cur_sym.type != sym_eof) {
            advance(cur_sym);
            while (cur_sym.value == "\n") advance(cur_sym);
            std::cout << "top:" << std::endl;
            std::cout << cur_sym << std::endl;
            if (cur_sym.value == "define") { // #define statement
                header += compile_macro(cur_sym);
            }
            // else if (cur_sym.value == "struct") {
            //     body += compile_struct(pos);
            // }
            else break;
        }
        return header + body;
    }
    std::string Compiler::compile_macro(Symbol& sym) {
        assert(sym.value == "define");
        std::string statement = "#define " + advance(sym).value + " ";
        while (sym.value != "\n") {
            advance(sym);
            std::cout << sym << std::endl;
            if (sym.type == sym_operator) statement += " " + sym.value + " ";
            else statement += sym.value;
        }
        std::cout << "ending sym: " << sym << std::endl;
        return statement;
    }
    std::string Compiler::compile_struct(int& pos) {
        std::string struct_def = "struct ";
    }
    std::string Compiler::compile_block(int& pos, int tabs) {

    }
    Symbol Compiler::advance(Symbol& sym) {
        return sym = next_symbol(sym.pos + 1);
    }
    Symbol Compiler::peek(Symbol sym) {
        return next_symbol(sym.pos + 1);
    }
    Symbol Compiler::next_symbol(int pos) {
        if (pos >= tokens.size()) throw std::string("Expected token after " + tokens.back().value);
        if (tokens[pos].type == Lexer::tok_type) {
            if (tokens[pos].value == "long") { // expect long long
                if (++pos >= tokens.size() || tokens[pos].value != "long") {
                    throw std::string("token 'long' must be extended to 'long long'.");
                }
                return Symbol{pos, 2, sym_type, "long long"};
            }
            return Symbol{pos, 1, sym_type, tokens[pos].value}; 
        }
        if (tokens[pos].type == Lexer::tok_keyword) {
            return Symbol{pos, 1, sym_keyword, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_whitespace) {
            return Symbol{pos, 1, sym_whitespace, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_identifier) { // function, function call, or assign
            if (pos + 1 >= tokens.size() || tokens[pos + 1].value != "(") {
                return Symbol{pos, 1, sym_identifier, tokens[pos].value};
            }
            std::string func = tokens[pos].value + "(";
            int tok_count = 2; // number of tokens
            pos += 2; // skip left paren
            SymbolType func_type;
            if (tokens[pos].value == ")") { // could be function declaration or function call
                if (pos + 1 >= tokens.size() || tokens[pos + 1].value != ":") func_type = sym_func_call;
                return Symbol{pos, 3, func_type, func + ")"};
            }
            if (tokens[pos].type == Lexer::tok_identifier) { // function call (e.g. SQUARE(x))
                func_type = sym_func_call;
            } else func_type = sym_func; // function signature (e.g. square(int x))
            while (tokens[pos].value != ")") {
                tok_count++;
                func += tokens[pos].value;
                pos++;
            }
            return Symbol{pos, tok_count + 1, func_type, func + ")"};
        }
        if (tokens[pos].type == Lexer::tok_char) {
            return Symbol{pos, 1, sym_char, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_operator) {
            return Symbol{pos, 1, sym_operator, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_eof) {
            return Symbol{pos, 1, sym_eof, tokens[pos].value};
        }
    }
}