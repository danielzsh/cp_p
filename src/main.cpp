#include <iostream>
#include <fstream>
#include "Compiler.h"
std::string readFile(const std::string& filename)
{
    std::ifstream source(filename);
    return std::string((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
}
int main(int argc, char** argv) {
    std::freopen("output.txt", "w", stdout);
    std::string input;
    if (argc <= 1) {
        std::cout << "provide path to source file\n";
        return 1;
    }
    if (argc > 1) input = readFile(argv[1]);
    // Lexer::Lexer lexer(input);
    try {
        std::cout << Compiler::compile(input);
        // auto tokens = lexer.lex();
        // for (auto token : tokens) std::cout << token << std::endl;
    } catch(std::string e) {
        std::cout << e << std::endl;
        return 1;
    }
    return 0;
}
