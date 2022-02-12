/**
    Implementation of Assembler.h
*/

#include "Assembler.h"

/* =========== PRIVATE ============= */

bool Assembler::isASMFile(const std::string path) const {
    return path.find(".asm") != std::string::npos;
}

void Assembler::writeACommand() {
    try {
        output_ << "0";
        output_ << code_->address(std::stoi(parser_->symbol()));
    } catch (FunctionCallException& e) {
        throw e;
    } catch (std::exception& e) {
        throw translateException("ADDRESS: " + parser_->symbol() + "(Parse line: " + std::to_string(parser_->getFileLine()) + ")");
    }
}

void Assembler::writeCCommand() {
    try {
        output_ << "111";
        output_ << code_->comp(parser_->comp());
        output_ << code_->dest(parser_->dest());
        output_ << code_->jump(parser_->jump());
    } catch (FunctionCallException& e) {
        throw e;
    } catch (std::exception& e) {
        throw translateException("COMP: " + parser_->comp() + ", DEST: " + parser_->dest() + ", JUMP: " + parser_->jump() + "(Parse line: " + std::to_string(parser_->getFileLine()) + ")");
    }
}

/* =========== PUBLIC ============= */

Assembler::Assembler(std::string path) {
    if (!isASMFile(path)) throw fileException(path);
    parser_ = new Parser(path);
    code_ = new Code();
    path.erase(path.find(".asm"), std::string::npos);
    path.append(".hack");
    output_.open(path);
}

Assembler::~Assembler() {
    delete parser_;
    delete code_;
    output_.close();
}

void Assembler::translate() {
    while (parser_->hasMoreCommands()) {
        parser_->advance();
        if (parser_->commandType() == CommandType::address) writeACommand();
        else if (parser_->commandType() == CommandType::compute) writeCCommand();
        output_ << std::endl;
    }
}