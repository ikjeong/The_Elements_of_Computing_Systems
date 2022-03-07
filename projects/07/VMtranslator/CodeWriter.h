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

    /* High level commands */
    void writePush(const std::string& segment, int index=-1) {
        if (segment == "D") {
            loadSPToA();
            output_ << "M=D" << "\n";
            increaseSP();
        } else if (segment == "constant") {
            output_ << "@" << index << "\n";
            output_ << "D=A" << "\n";
            writePush("D");
        } else {
            throw translate_exception("can't PUSH to " + segment);
        }
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
        } else {
            throw translate_exception("can't POP to " + segment);
        }
    }

    void writeBooleanLogic(const std::string& jump) {
        writePushPop(CommandType::C_POP, "D");
        writePushPop(CommandType::C_POP, "A");
        output_ << "D=D-A" << "\n";
        output_ << "@LABEL" << label_count_ << "\n";
        output_ << "D;" << jump << "\n";
        output_ << "D=0" << "\n";
        output_ << "@END_LABEL" << label_count_ << "\n";
        output_ << "0;JMP" << "\n";
        output_ << "(LABEL" << label_count_ << ")" << "\n";
        output_ << "D=-1" << "\n";
        output_ << "(END_LABEL" << label_count_ << ")" << "\n";
        writePushPop(CommandType::C_PUSH, "D");
        ++label_count_;
    }

    bool isVMFile(const std::string path) const {
        return path.find(".vm") != std::string::npos;
    }

public:
    CodeWriter(std::string path) {
        /* TODO: directory */
        if (!isVMFile(path)) throw file_exception(path);
        path.erase(path.find(".vm"), std::string::npos);
        path.append(".asm");
        output_.open(path);
        if (output_.fail()) throw file_exception(path);
        label_count_ = 0;
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
            writePushPop(CommandType::C_POP, "D");
            writePushPop(CommandType::C_POP, "A");
            output_ << "D=D+A" << "\n";
            writePushPop(CommandType::C_PUSH, "D");
        } else if (command == "sub") {
            writePushPop(CommandType::C_POP, "D");
            writePushPop(CommandType::C_POP, "A");
            output_ << "D=D-A" << "\n";
            writePushPop(CommandType::C_PUSH, "D"); 
        } else if (command == "neg") {
            writePushPop(CommandType::C_POP, "D");
            output_ << "D=-D" << "\n";
            writePushPop(CommandType::C_PUSH, "D");
        } else if (command == "eq") {
            writeBooleanLogic("JEQ");
        } else if (command == "gt") {
            writeBooleanLogic("JGT");
        } else if (command == "lt") {
            writeBooleanLogic("JLT");
        } else if (command == "and") {
            writePushPop(CommandType::C_POP, "D");
            writePushPop(CommandType::C_POP, "A");
            output_ << "D=D&A" << "\n";
            writePushPop(CommandType::C_PUSH, "D"); 
        } else if (command == "or") {
            writePushPop(CommandType::C_POP, "D");
            writePushPop(CommandType::C_POP, "A");
            output_ << "D=D|A" << "\n";
            writePushPop(CommandType::C_PUSH, "D"); 
        } else if (command == "not") {
            writePushPop(CommandType::C_POP, "D");
            output_ << "D=!D" << "\n";
            writePushPop(CommandType::C_PUSH, "D");
        } else {
            throw translate_exception(command);
        }
    }

    void writePushPop(const CommandType& command, const std::string& segment, int index=-1)  {
        if (command == CommandType::C_PUSH) writePush(segment, index);
        else if (command == CommandType::C_POP) writePop(segment, index);
        else throw translate_exception("NOT PUSH/POP COMMAND");
    }

    void close() {
        if (output_.is_open()) output_.close();
    }
};

#endif