/**
    Assembler

    Function: 
    - constructor:
        Argument is .asm file path.
        Constructor set file path to parser,
        and make new file(.hack).
    - translate:
        tranlaste .asm to .hack binary code file.
*/

#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include "Global.h"
#include "Parser.h"
#include "Code.h"

class Assembler {
private:
    Parser* parser;
    Code* code;
    std::ofstream output;

public:
    Assembler(std::string path) {
        parser = new Parser(path);
        code = new Code();
        path.erase(path.find(".asm"), std::string::npos);
        path.append(".hack");
        output.open(path);
    }

    ~Assembler() {
        delete parser;
        delete code;
        output.close();
    }

    void translate() {
        while (parser->hasMoreCommands()) {
            if (parser->commandType() == CommandType::label || parser->commandType() == CommandType::address) {
                output << "0";
                output << code->address(std::stoi(parser->symbol()));
            } else if (parser->commandType() == CommandType::compute) {
                output << "111";
                output << code->dest(parser->dest());
                output << code->comp(parser->comp());
                output << code->jump(parser->jump());
            }
            output << std::endl;
            parser->advance();
        }
    }
};

#endif