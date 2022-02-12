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

class Assembler {
private:
    Parser* parser_;
    Code* code_;
    std::ofstream output_;

private:
    bool isASMFile(const std::string path) const;
    void writeACommand();
    void writeCCommand();

public:
    Assembler(std::string path);
    ~Assembler();
    void translate();
};

#endif