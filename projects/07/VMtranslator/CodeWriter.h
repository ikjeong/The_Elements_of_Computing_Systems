/**
    CodeWriter Module(Class)

    Routines
    - setFileName
    - writeArithmetic
    - writePushPop
    - close

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

    if you want to pop to A,
    it is the last command to use.
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

    /* High level commands */
    void writePush(const std::string& segment, int index=-1);
    void writePop(const std::string& segment, int index=-1);
    void writeBooleanLogic(const std::string& jump);

    bool isVMFile(const std::string& path) const;

public:
    CodeWriter(std::string path);
    ~CodeWriter();
    void setFileName(std::string path);
    void writeArithmetic(const std::string& command);
    void writePushPop(const CommandType& command, const std::string& segment, int index=-1);
    void close();
};

#endif