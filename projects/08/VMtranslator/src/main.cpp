/**
    Main Virtual Machine
    v1: Implement arithmetic command and memory access command.
    v2: Implement program flow command and function calling command.

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

    Program flow command
    - label label: Label the current function code location. The valid range is inside the defined function.
                   The name does not start with numbers, but is any string composed of letters, numbers, underscores, periods, and colons.
                   It translate to functionName$label.
    - goto label: unconditional goto command
    - if-goto label: Pop value from stack. If it isn't 0, goto label.

    Function call command
    Function name does not start with numbers, but is any string composed of letters, numbers, underscores, periods, and colons.
    The range of function names is global.
    - function f n: definition of function(consist of arguments(number of n)).
    - call f m: call function with arguments(number of m).
    - return: return function.

    Caller
    - Before calling function, it needs to push args, stats to stack.
    - Call function.
    - Top of stack is return value of function.
    - The memory segments(excluding temp) are same as before the call.

    Called
    - The instrument segment is initialized by the caller, 
      and the local variable segment is assigned and initialized to 0.
      The static segment is assigned to the static segment of the VM file to which the function belongs.
      This, that, pointer, and temp are undefined.
    - Before return, function need to push return value to stack.

    Initialization
    - Program always start Sys.init function at first.
    - Bootstrap code: SP=256
                      call Sys.init

    Global Stack
    - A memory resource for a function is defined as a frame. 
      The frame consists of arguments used, pointers storing the state of the called function,
      regional variables of the called function, and a task stack.

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