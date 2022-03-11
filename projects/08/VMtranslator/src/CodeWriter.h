/**
    CodeWriter Module(Class)

    Routines
    - setFileName
    - writeArithmetic
    - writePushPop
    - close
    - writeInit
    - writeLabel
    - writeGoto
    - writeIf
    - writeCall
    - writeReturn
    - writeFunction

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
*/

#ifndef __CODE_WRITER_H__
#define __CODE_WRITER_H__

#include "Global.h"

class CodeWriter {
private:
    std::ofstream output_;
    std::string file_name_;
    int label_count_;

    /* Low level commands */
    void decreaseSP();
    void increaseSP();
    void loadSPToA();
    void loadSegmentToA(const std::string& segment, int index);

    /* High level commands */
    void writePush(const std::string& segment, int index=-1);
    void writePop(const std::string& segment, int index=-1);
    void writeBooleanLogic(const std::string& jump);

    bool isVMFile(const std::string& path) const;

public:
    CodeWriter(std::string path);
    ~CodeWriter();
    void setFileName(std::string path);
    void writeInit();
    void writeArithmetic(const std::string& command);
    void writePushPop(const CommandType& command, const std::string& segment, int index=-1);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeCall(const std::string& functionName, int numArgs);
    void writeReturn();
    void writeFunction(const std::string& functionName, int numLocals);
    void close();
};

#endif