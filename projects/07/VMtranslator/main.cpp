/**
    Main Virtual Machine
    v1: Implement arithmetic command and memory access command.
    todo: Implement program flow command and function calling command.

    Command structure
    command, command arg or command arg1 arg2

    Arithmetic command
    - add
    - sub
    - neg
    - eq
    - gt
    - lt
    - and
    - or
    - not

    Memory access command
    - push segment index
    - pop segment index

    Virtual Memory Segment
    Virtual Machine manipulate virtual memory segments.
    The left side is a memory segment, and the right side is a memory corresponding to the segment.
    '*' is the value indicated by the corresponding memory.
    - argument  *ARG(RAM[2])
    - local     *LCL(RAM[1])
    - static    RAM[16-255]
    - constant  (This segment is emulated.)
    - this      *THIS(RAM[3])
    - that      *THAT(RAM[4])
    - pointer   0: THIS(RAM[3]), 1: THAT(RAM[4])
    - temp      RAM[5-12]
    + Virtualized stack     *SP(RAM[0]) -> RAM[256-2047]

    Modules
    - Parser: After parsing, access to each field is provided.
    - CodeWriter: Returns the binary code for the association symbol.

    How to use
    prompt> VMtranslator source
    source is .vm file or directory which contains .vm files.
    return .asm file.
*/

#include "Global.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
    
    Parser* p = new Parser(argv[1]);

    while (p->hasMoreCommands()) {
        p->advance();
        if (p->commandType() == CommandType::C_ARITHMETIC) std::cout << "ARITHMETIC";
        else if (p->commandType() == CommandType::C_PUSH) std::cout << "PUSH";
        else if (p->commandType() == CommandType::C_POP) std::cout << "POP"; 
        std::cout << " ";
        std::cout << p->arg1() << " ";
        if (p->arg2()!=-1) std::cout << p->arg2();
        std::cout << std::endl;
    }
    
    delete p;

    return 0;
}