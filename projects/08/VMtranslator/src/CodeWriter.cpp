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
    output_ << "D=M" << "\n";
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
        writePush("D");
        return;
    }

    /* Load segment to A */
    if (segment == "local") loadSegmentToA("LCL", index);
    else if (segment == "argument") loadSegmentToA("ARG", index);
    else if (segment == "this") loadSegmentToA("THIS", index);
    else if (segment == "that") loadSegmentToA("THAT", index);
    else if (segment == "pointer") {
        if (index < 0 || index > 1) throw translate_exception("can't use pointer " + index);
        output_ << "@R" << 3+index << "\n";
    } else if (segment == "temp") {
        if (index < 0 || index > 7) throw translate_exception("can't use temp " + index);
        output_ << "@R" << 5+index << "\n";
    } else if (segment == "static") {
        output_ << "@" << file_name_ << "." << index << "\n";
    } else throw translate_exception("can't PUSH to " + segment);

    /* Load segment to D, and Push D to Stack */
    output_ << "D=M" << "\n";
    writePush("D");
}

void CodeWriter::writePop(const std::string& segment, int index) {
    if (segment == "A") {
        decreaseSP();
        loadSPToA();
        output_ << "A=M" << "\n";
        return;
    }
    if (segment == "D") {
        decreaseSP();
        loadSPToA();
        output_ << "D=M" << "\n";
        return;
    }

    /* Load segment to A */
    if (segment == "local") loadSegmentToA("LCL", index);
    else if (segment == "argument") loadSegmentToA("ARG", index);
    else if (segment == "this") loadSegmentToA("THIS", index);
    else if (segment == "that") loadSegmentToA("THAT", index);
    else if (segment == "pointer") {
        if (index < 0 || index > 1) throw translate_exception("can't use pointer " + index);
        output_ << "@R" << 3+index << "\n";
    } else if (segment == "temp") {
        if (index < 0 || index > 7) throw translate_exception("can't use temp " + index);
        output_ << "@R" << 5+index << "\n";
    } else if (segment == "static") {
        output_ << "@" << file_name_ << "." << index << "\n";
    } else throw translate_exception("can't POP to " + segment);

    /* save A to R13 */
    output_ << "D=A" << "\n";
    output_ << "@R13" << "\n";
    output_ << "M=D" << "\n";

    /* Pop stack to D and save D to segment(R13) */
    writePop("D");
    output_ << "@R13" << "\n";
    output_ << "A=M" << "\n";
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
    writeInit();
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
    function_name_ = "";
    output_ << "// Translate " << file_name_ << ".vm" << "\n";
}

void CodeWriter::writeInit() {
    output_ << "// Bootstrap code" << "\n";
    output_ << "@256" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@SP" << "\n";
    output_ << "M=D" << "\n";
    writeCall("Sys.init", 0);
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

void CodeWriter::writeLabel(const std::string& label) {
    output_ << "(" << function_name_ << "$" << label << ")" << "\n";
}

void CodeWriter::writeGoto(const std::string& label) {
    output_ << "@" << function_name_ << "$" << label << "\n";
    output_ << "0;JMP" << "\n";
}

void CodeWriter::writeIf(const std::string& label) {
    writePop("D");
    output_ << "@" << function_name_ << "$" << label << "\n";
    output_ << "D;JNE" << "\n";
}

void CodeWriter::writeCall(const std::string& functionName, int numArgs) {
    // push return-address
    output_ << "@RETURN" << label_count_ << "\n";
    output_ << "D=A" << "\n";
    writePush("D");

    // push LCL
    output_ << "@LCL" << "\n";
    output_ << "D=M" << "\n";
    writePush("D");

    // push ARG
    output_ << "@ARG" << "\n";
    output_ << "D=M" << "\n";
    writePush("D");

    // push THIS
    output_ << "@THIS" << "\n";
    output_ << "D=M" << "\n";
    writePush("D");

    // push THAT
    output_ << "@THAT" << "\n";
    output_ << "D=M" << "\n";
    writePush("D");

    // set ARG
    output_ << "@SP" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@" << numArgs << "\n";
    output_ << "D=D-A" << "\n";
    output_ << "@5" << "\n";
    output_ << "D=D-A" << "\n";
    output_ << "@ARG" << "\n";
    output_ << "M=D" << "\n";

    // set LCL
    loadSPToA();
    output_ << "D=A" << "\n";
    output_ << "@LCL" << "\n";
    output_ << "M=D" << "\n";

    // goto function
    output_ << "@" << functionName << "\n";
    output_ << "0;JMP" << "\n";

    // (return-address)
    output_ << "(" << "RETURN" << label_count_ << ")" << "\n";
    ++label_count_;
}

void CodeWriter::writeReturn() {
    // save LCL to R14
    output_ << "@LCL" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@R14" << "\n";
    output_ << "M=D" << "\n";

    // save return-address to R15
    output_ << "@5" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@R14" << "\n";
    output_ << "D=M-D" << "\n";
    output_ << "A=D" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@R15" << "\n";
    output_ << "M=D" << "\n";

    // set return value -> R13 used.
    writePop("argument", 0);

    // restore SP
    output_ << "@ARG" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@SP" << "\n";
    output_ << "M=D+1" << "\n";

    // restore THAT
    output_ << "@1" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@R14" << "\n";
    output_ << "D=M-D" << "\n";
    output_ << "A=D" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@THAT" << "\n";
    output_ << "M=D" << "\n";

    // restore THIS
    output_ << "@2" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@R14" << "\n";
    output_ << "D=M-D" << "\n";
    output_ << "A=D" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@THIS" << "\n";
    output_ << "M=D" << "\n";

    // restore ARG
    output_ << "@3" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@R14" << "\n";
    output_ << "D=M-D" << "\n";
    output_ << "A=D" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@ARG" << "\n";
    output_ << "M=D" << "\n";

    // restore LCL
    output_ << "@4" << "\n";
    output_ << "D=A" << "\n";
    output_ << "@R14" << "\n";
    output_ << "D=M-D" << "\n";
    output_ << "A=D" << "\n";
    output_ << "D=M" << "\n";
    output_ << "@LCL" << "\n";
    output_ << "M=D" << "\n";

    // goto return-address
    output_ << "@R15" << "\n";
    output_ << "A=M" << "\n";
    output_ << "0;JMP" << "\n";
}

void CodeWriter::writeFunction(const std::string& functionName, int numLocals) {
    function_name_ = functionName;
    output_ << "(" <<  function_name_ << ")" << "\n";
    for (int i = 0; i < numLocals; ++i) writePush("constant", 0);
}

void CodeWriter::close() {
    if (output_.is_open()) output_.close();
}