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

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    
    std::ifstream input(argv[1]);
    if (input.fail()) {
        std::cout << "Can't find file" << std::endl;
        exit(100);
    }

    std::string s;
    while (!input.eof()) {
        input >> s;
        std::cout << s << std::endl;
    }
    
    input.close();
    return 0;
}