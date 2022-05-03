/**
 * Implementation of VMWriter.h
 */

#include "VMWriter.h"

/* =========== PRIVATE ============= */

bool VMWriter::isJackFile(const std::string& path) const {
    return path.find(".jack") != std::string::npos;
}

/* =========== PUBLIC ============= */

VMWriter::VMWriter() {

}

VMWriter::~VMWriter() {
    if (output_.is_open()) output_.close();
}

void VMWriter::setOutputFile(std::string path) {
    if (output_.is_open()) output_.close();
    if (!isJackFile(path)) throw file_exception(path);
    path.erase(path.find(".jack"), std::string::npos);
    path.append(".vm");
    output_.open(path);
    if (output_.fail()) throw file_exception(path);
}

void VMWriter::close() {
    if (output_.is_open()) output_.close();
}

void VMWriter::writePush(const Segment segment, const int index) {
    output_ << "push " << segmentToString(segment) << " " << index << std::endl;
}

void VMWriter::writePop(const Segment segment, const int index) {
    output_ << "pop " << segmentToString(segment) << " " << index << std::endl;
}

void VMWriter::writeArithmetic(const Command command) {
    output_ << commandToString(command) << std::endl;
}

void VMWriter::writeLabel(const std::string& label) {
    output_ << "label " << label << std::endl;
}

void VMWriter::writeGoto(const std::string& label) {
    output_ << "goto " << label << std::endl;
}

void VMWriter::writeIf(const std::string& label) {
    output_ << "if-goto " << label << std::endl;
}

void VMWriter::writeCall(const std::string& name, const int nArgs) {
    output_ << "call " << name << " " << nArgs << std::endl;
}

void VMWriter::writeFunction(const std::string& name, const int nLocals) {
    output_ << "function " << name << " " << nLocals << std::endl;
}

void VMWriter::writeReturn() {
    output_ << "return" << std::endl;
}