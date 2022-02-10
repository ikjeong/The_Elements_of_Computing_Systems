/**
    Main assembler program

    Modules
    - Parser: After parsing, access to each field is provided.
    - Code: Returns the binary code for the association symbol.
    - SymbolTable: Symbol table creation and symbol processing are performed.

    Process
    1. Initialization: Process declaration symbols.
    2. 1PASS: A symbol table is configured for a label(pseudo code).
    3. 2PASS: Translate each command into binary code.

    How to use
    prompt> Assember filePath
*/

#include "Global.h"
#include "Parser.h"
#include "Code.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No input file" << std::endl;
        exit(100);
    } 
    Parser parser(argv[1]);
    Code code;

    while (parser.hasMoreCommands()) {
        if (parser.commandType() == CommandType::label || parser.commandType() == CommandType::address) {
            std::cout << "0" << code.address(std::stoi(parser.symbol())) << std::endl;
        } else if (parser.commandType() == CommandType::compute) {
            std::cout << "111" << code.dest(parser.dest());
            std::cout << code.comp(parser.comp());
            std::cout << code.jump(parser.jump()) << std::endl;
        }
        parser.advance();
    }

    return 0;
}