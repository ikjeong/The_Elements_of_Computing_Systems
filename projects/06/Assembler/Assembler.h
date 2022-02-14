/**
    Assembler

    Function: 
    - constructor:
        Argument is .asm file path.
        Constructor set file path to parser_,
        and make new file(.hack).
    - translate:
        tranlaste .asm to .hack binary code_ file.
*/

#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "Global.h"
#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

class Assembler {
private:
    Parser* parser_;
    Code* code_;
    SymbolTable* symbol_table_;
    std::ofstream output_;

private:
    bool isASMFile(const std::string path) const;
    int writePart(SymbolType type);
    void writeACommand();
    void writeCCommand();
    void pass1();
    void pass2();

public:
    Assembler(std::string path);
    ~Assembler();
    void translate();
};

#endif