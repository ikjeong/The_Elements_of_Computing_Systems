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
    prompt> Assembler filePath
*/

#include "Global.h"
#include "Assembler.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No input file" << std::endl;
        exit(static_cast<int>(Error::fileOpen));
    } 
    Assembler assembler(argv[1]);
    assembler.translate();

    return 0;
}