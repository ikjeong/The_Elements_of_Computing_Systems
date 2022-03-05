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

*/

#ifndef __CODE_WRITER_H__
#define __CODE_WRITER_H__

#include "Global.h"

class CodeWriter {
private:
    std::ofstream output_;
    std::string file_name_;



    void decreaseSP() {
        output_ << "@SP" << "\n";
        output_ << "M=M-1" << "\n";
    }

    void increaseSP() {
        output_ << "@SP" << "\n";
        output_ << "M=M+1" << "\n";
    }

    void loadSPToA() {
        output_ << "@SP" << "\n";
        output_ << "A=M" << "\n";
    }

    void saveDToStack() {
        loadSPToA();
        output_ << "M=D" << "\n";
        increaseSP();
    }

    void writePop(const std::string& segment, int index=-1) {
        if (segment == "D") {
            decreaseSP();
            loadSPToA();
            output_ << "D=M" << "\n";
        } else if (segment == "A") {
            decreaseSP();
            loadSPToA();
            output_ << "A=M" << "\n";
        }
    }

    bool isVMFile(const std::string path) const {
        return path.find(".vm") != std::string::npos;
    }

public:
    CodeWriter(std::string path) {
        if (!isVMFile(path)) throw file_exception(path);
        path.erase(path.find(".vm"), std::string::npos);
        path.append(".asm");
        output_.open(path);
        if (output_.fail()) throw file_exception(path);
    }

    ~CodeWriter() {
        close();
    }

    void setFileName(std::string path) {
        if (!isVMFile(path)) throw file_exception(path);
        path.erase(path.find(".vm"), std::string::npos);
        for (int i = path.size()-1; i >= 0; --i) {
            if (path[i] == '/') {
                path.erase(0, i+1);
                break;
            }
        }
        file_name_ = path;
    }

    void writeArithmetic(const std::string& command) {
        if (command == "add") {
            writePop("D");
            writePop("A");
            output_ << "D=D+A" << "\n";
            saveDToStack();
        } else if (command == "sub") {
            writePop("D");
            writePop("A");
            output_ << "D=D-A" << "\n";
            saveDToStack(); 
        } else if (command == "neg") {
            writePop("D");
            output_ << "D=-D" << "\n";
            saveDToStack();
        } else if (command == "eq") {

        }
    }

    void writePushPop(const CommandType& command, const std::string& segment, int index=-1)  {
        if (command == CommandType::C_PUSH) output_ << "PUSH" << "\n";
        else if (command == CommandType::C_POP) output_ << "POP" << "\n";
    }

    void close() {
        if (output_.is_open()) output_.close();
    }
};

#endif