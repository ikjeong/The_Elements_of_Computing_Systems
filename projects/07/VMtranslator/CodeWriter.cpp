/**
    Implementation of CodeWriter.h
*/

#include "CodeWriter.h"

/* =========== PRIVATE ============= */

/* Low level commands */
void CodeWriter::decreaseSP() {
    output_ << "@SP" << "\n";
    output_ << "M=M-1" << "\n";
}

void CodeWriter::increaseSP() {
    output_ << "@SP" << "\n";
    output_ << "M=M+1" << "\n";
}

void CodeWriter::loadSPToA() {
    output_ << "@SP" << "\n";
    output_ << "A=M" << "\n";
}

void CodeWriter::loadSegmentToA(const std::string& segment, int index) {
    output_ << "@" << segment << "\n";
    output_ << "D=A" << "\n";
    output_ << "@" << index << "\n";
    output_ << "A=D+A" << "\n";
}

/* High level commands */
void CodeWriter::writePush(const std::string& segment, int index) {
    if (segment == "D") {
        loadSPToA();
        output_ << "M=D" << "\n";
        increaseSP();
        return;
    }
    
    if (segment == "constant") {
        output_ << "@" << index << "\n";
        output_ << "D=A" << "\n";
    } else if (segment == "local") {
        loadSegmentToA("LCL", index);
        output_ << "D=M" << "\n";
    } else if (segment == "argument") {
        loadSegmentToA("ARG", index);
        output_ << "D=M" << "\n";
    } else if (segment == "this") {
        loadSegmentToA("THIS", index);
        output_ << "D=M" << "\n";
    } else if (segment == "that") {
        loadSegmentToA("THAT", index);
        output_ << "D=M" << "\n";
    } else {
        throw translate_exception("can't PUSH to " + segment);
    }
    writePush("D");
}

void CodeWriter::writePop(const std::string& segment, int index) {
    decreaseSP();
    loadSPToA();

    if (segment == "A") {
        output_ << "A=M" << "\n";
        return;
    }

    output_ << "D=M" << "\n";
    if (segment == "D") return;
    else if (segment == "local") loadSegmentToA("LCL", index);
    else if (segment == "argument") loadSegmentToA("ARG", index);
    else if (segment == "this") loadSegmentToA("THIS", index);
    else if (segment == "that") loadSegmentToA("THAT", index);
    else throw translate_exception("can't POP to " + segment);
    output_ << "M=D" << "\n";
}

void CodeWriter::writeBooleanLogic(const std::string& jump) {
    writePushPop(CommandType::C_POP, "D");
    writePushPop(CommandType::C_POP, "A");
    output_ << "D=A-D" << "\n";
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

bool CodeWriter::isVMFile(const std::string& path) const {
    return path.find(".vm") != std::string::npos;
}

/* =========== PUBLIC ============= */

CodeWriter::CodeWriter(std::string path) {
    if (isVMFile(path)) path.erase(path.find(".vm"), std::string::npos);
    if (path.back() == '/') path.pop_back();
    path.append(".asm");
    output_.open(path);
    if (output_.fail()) throw file_exception(path);
    label_count_ = 0;
}

CodeWriter::~CodeWriter() {
    close();
}

void CodeWriter::setFileName(std::string path) {
    if (!isVMFile(path)) throw file_exception(path);
    path.erase(path.find(".vm"), std::string::npos);
    for (int i = path.size()-1; i >= 0; --i) {
        if (path[i] == '/') {
            path.erase(0, i+1);
            break;
        }
    }
    file_name_ = path;
    output_ << "// Translate " << file_name_ << ".vm" << "\n";
}

void CodeWriter::writeArithmetic(const std::string& command) {
    if (command == "add") {
        writePushPop(CommandType::C_POP, "D");
        writePushPop(CommandType::C_POP, "A");
        output_ << "D=A+D" << "\n";
        writePushPop(CommandType::C_PUSH, "D");
    } else if (command == "sub") {
        writePushPop(CommandType::C_POP, "D");
        writePushPop(CommandType::C_POP, "A");
        output_ << "D=A-D" << "\n";
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
        output_ << "D=A&D" << "\n";
        writePushPop(CommandType::C_PUSH, "D"); 
    } else if (command == "or") {
        writePushPop(CommandType::C_POP, "D");
        writePushPop(CommandType::C_POP, "A");
        output_ << "D=A|D" << "\n";
        writePushPop(CommandType::C_PUSH, "D"); 
    } else if (command == "not") {
        writePushPop(CommandType::C_POP, "D");
        output_ << "D=!D" << "\n";
        writePushPop(CommandType::C_PUSH, "D");
    } else {
        throw translate_exception(command);
    }
}

void CodeWriter::writePushPop(const CommandType& command, const std::string& segment, int index)  {
    if (command == CommandType::C_PUSH) writePush(segment, index);
    else if (command == CommandType::C_POP) writePop(segment, index);
    else throw translate_exception("NOT PUSH/POP COMMAND");
}

void CodeWriter::close() {
    if (output_.is_open()) output_.close();
}