#include "Compiler.h"
#include <assert.h>
namespace Compiler {
    std::ostream &operator<<(std::ostream &os, Symbol const& sym) {
        std::string value = sym.value;
        if (value == "\n") value = "\\n";
        if (value == "\t") value = "\\t"; 
        return os << sym.pos << " " << sym.type << " " << value;
    }
    Compiler::Compiler(std::string input) {
        Lexer::Lexer lexer(input);
        tokens = lexer.lex();
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }

    /// @brief compile source file
    /// @return compiled source file
    std::string Compiler::compile() {
        Symbol cur_sym = {-1, sym_start, ""};
        while (cur_sym.type != sym_eof) {
            advance(cur_sym);
            while (cur_sym.value == "\n") advance(cur_sym);
            std::cout << "top:" << std::endl;
            std::cout << cur_sym << std::endl;
            if (cur_sym.value == "define") { // #define statement
                header += compile_macro(cur_sym);
            }
            else if (cur_sym.value == "struct") {
                body += compile_struct(cur_sym);
            }
            else break;
        }
        return header + "\n" + body;
    }

    /// @brief compiles a macro
    /// @param sym first symbol of macro (should be "define")
    /// @return compiled macro
    std::string Compiler::compile_macro(Symbol& sym) {
        assert(sym.value == "define");
        advance(sym);
        std::string statement = "#define " + sym.value + " ";
        statement += copy_line(advance(sym)) + "\n"; 
        // std::string statement = "#define " + sym.value + " " + copy_line(advance(sym)) + "\n";
        // ^ this doesn't work for some reason (probably some stupid reference thing)
        return statement;
    }

    /// @brief compiles struct definition
    /// @param sym first symbol in struct (should be "struct")
    /// @return compiled struct
    std::string Compiler::compile_struct(Symbol& sym) {
        assert(sym.value == "struct");
        std::string struct_def = "struct ";
        if (peek(sym).type != sym_identifier) {
            throw std::string("expected identifier after keyword 'struct', got " + sym.value + " instead.");
        }
        struct_def += advance(sym).value + " ";
        if (advance(sym).value != ":") {
            throw std::string("expected colon after " + struct_def + " declaration.");
        }
        struct_def +=   "{\n" +
                            compile_block(advance(advance(sym)), 1) +
                        "};\n";
        return struct_def;
    }

    /// @brief compiles an indented block
    /// @param sym first symbol in block (should be an indent)
    /// @param tabs level of indentation of block
    /// @return compiled block (tabs included)
    std::string Compiler::compile_block(Symbol& sym, int tabs) {
        std::string block = "";
        while (sym.value == "\t") {
            for (int i = 0; i < tabs; i++) {
                assert(sym.value == "\t");
                block += "\t";
                advance(sym);
            }
            block += compile_decl(sym);
            advance(sym);
        }
        return block;
    }
    
    /// @brief compiles a variable or function declaration
    /// @param sym first symbol in declaration
    /// @return compiled declaration
    std::string Compiler::compile_decl(Symbol& sym) {
        std::string decl = "";
        if (sym.type == sym_type) { // variable declaration
            std::cout << "variable declaration " + sym.value << "\n";
            decl = sym.value + " ";
            decl += copy_line(advance(sym)) + ";\n";
            // decl = sym.value + " " + copy_line(sym) + ";\n";
            // ^ this doesn't work either
            // i know c++ sucks
        }
        // TODO: add function declaration later
        return decl;
    }

    /// @brief copies tokens verbatim until end of line
    /// @param sym start of copying, will be advanced to newline symbol at function termination
    /// @return copied line without newline at the end
    std::string Compiler::copy_line(Symbol& sym) {
        std::string line = "";
        while (sym.value != "\n") {
            if (sym.type == sym_operator) line += " " + sym.value + " ";
            else if (sym.value == ",") line += sym.value + " ";
            else line += sym.value;
            advance(sym);
        }
        return line;
    }

    /// @brief advances symbol
    /// @param sym symbol to advance
    /// @return reference to advanced symbol
    Symbol& Compiler::advance(Symbol& sym) {
        return sym = next_symbol(sym.pos + 1);
    }

    /// @brief peek at next symbol
    /// @param sym symbol to peek after
    /// @return next symbol
    Symbol Compiler::peek(Symbol sym) {
        return next_symbol(sym.pos + 1);
    }

    /// @brief get symbol starting at given position
    /// @param pos position of first token in symbol
    /// @return symbol that starts at pos
    Symbol Compiler::next_symbol(int pos) {
        if (pos >= tokens.size()) throw std::string("Expected token after " + tokens.back().value);
        if (tokens[pos].type == Lexer::tok_type) {
            // TODO: consider adding ll instead of long long
            if (tokens[pos].value == "long") { // expect long long
                if (++pos >= tokens.size() || tokens[pos].value != "long") {
                    throw std::string("token 'long' must be extended to 'long long'.");
                }
                return Symbol{pos, sym_type, "long long"};
            }
            return Symbol{pos, sym_type, tokens[pos].value}; 
        }
        if (tokens[pos].type == Lexer::tok_keyword) {
            return Symbol{pos, sym_keyword, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_whitespace) {
            return Symbol{pos, sym_whitespace, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_identifier) { // function, function call, or assign
            if (pos + 1 >= tokens.size() || tokens[pos + 1].value != "(") {
                return Symbol{pos, sym_identifier, tokens[pos].value};
            }
            std::string func = tokens[pos].value + "(";
            int tok_count = 2; // number of tokens
            pos += 2; // skip left paren
            SymbolType func_type;
            if (tokens[pos].value == ")") { // could be function declaration or function call
                if (pos + 1 >= tokens.size() || tokens[pos + 1].value != ":") func_type = sym_func_call;
                return Symbol{pos, func_type, func + ")"};
            }
            if (tokens[pos].type == Lexer::tok_identifier) { // function call (e.g. SQUARE(x))
                func_type = sym_func_call;
            } else func_type = sym_func; // function signature (e.g. square(int x))
            while (tokens[pos].value != ")") {
                tok_count++;
                func += tokens[pos].value;
                pos++;
            }
            return Symbol{pos, func_type, func + ")"};
        }
        if (tokens[pos].type == Lexer::tok_char) {
            return Symbol{pos, sym_char, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_operator) {
            return Symbol{pos, sym_operator, tokens[pos].value};
        }
        if (tokens[pos].type == Lexer::tok_eof) {
            return Symbol{pos, sym_eof, tokens[pos].value};
        }
    }
}