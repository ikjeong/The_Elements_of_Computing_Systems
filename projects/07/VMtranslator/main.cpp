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
    - CodeWriter: Returns the assembly language.

    How to use
    prompt> VMtranslator source
    source is .vm file or directory which contains .vm files.
    return .asm file.
*/

#include "VMtranslator.h"

int main(int argc, char* argv[]) {
    try {
        VMtranslator translator(argv[1]);
        translator.translate();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}