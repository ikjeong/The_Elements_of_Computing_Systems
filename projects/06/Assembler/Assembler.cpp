/**
    Implementation of Assembler.h
*/

#include "Assembler.h"

/* =========== PRIVATE ============= */

bool Assembler::isASMFile(const std::string path) const {
    return path.find(".asm") != std::string::npos;
}

int Assembler::writePart(SymbolType type) {
    std::string (Code::*toBinary)(const std::string&) const;
    std::string (Parser::*syntax)() const;

    if (type == SymbolType::address) {
        toBinary = &Code::address;
        syntax = &Parser::symbol;
    } else if (type == SymbolType::comp) {
        toBinary = &Code::comp;
        syntax = &Parser::comp;
    } else if (type == SymbolType::dest) {
        toBinary = &Code::dest;
        syntax = &Parser::dest;
    } else if (type == SymbolType::jump) {
        toBinary = &Code::jump;
        syntax = &Parser::jump;
    } else {
        throw functionCallException("Incorrect Symbol Type (Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    }

    std::string result = "";
    if (code_->canTranslateToBinary((parser_->*syntax)(), type)) {
        result = (code_->*toBinary)((parser_->*syntax)());
    } else if (symbol_table_->contains((parser_->*syntax)())) {
        result = code_->address(symbol_table_->GetAddress((parser_->*syntax)()));
    } else {
        result = code_->address(symbol_table_->addVariable((parser_->*syntax)()));
    }
    output_ << result;
    return static_cast<int>(result.size());
}

void Assembler::writeACommand() {
    try {
        int length = 1;
        output_ << "0";
        length += writePart(SymbolType::address);
        if (length != 16) throw std::exception();
    } catch (functionCallException& e) {
        throw e;
    } catch (std::exception& e) {
        throw translateException("ADDRESS: " + parser_->symbol() + "(Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    }
}

void Assembler::writeCCommand() {
    try {
        int length = 3;
        output_ << "111";
        length += writePart(SymbolType::comp);
        length += writePart(SymbolType::dest);
        length += writePart(SymbolType::jump);
        if (length != 16) throw std::exception();
    } catch (functionCallException& e) {
        throw e;
    } catch (std::exception& e) {
        throw translateException("COMP: " + parser_->comp() + ", DEST: " + parser_->dest() + ", JUMP: " + parser_->jump() + "(Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    }
}

void Assembler::pass1() {
    while (parser_->hasMoreCommands()) {
        parser_->advance();
        if (parser_->commandType() == CommandType::label)
            symbol_table_->addEntry(parser_->symbol(), parser_->getCommandAddress()+1);
    }
    parser_->resetCursor();
}

void Assembler::pass2() {
    while (parser_->hasMoreCommands()) {
        parser_->advance();
        if (parser_->commandType() == CommandType::address) writeACommand();
        else if (parser_->commandType() == CommandType::compute) writeCCommand();
        else continue;
        output_ << std::endl;
    }
}

/* =========== PUBLIC ============= */

Assembler::Assembler(std::string path) {
    if (!isASMFile(path)) throw fileException(path);
    parser_ = new Parser(path);
    code_ = new Code();
    symbol_table_= new SymbolTable();
    path.erase(path.find(".asm"), std::string::npos);
    path.append(".hack");
    output_.open(path);
}

Assembler::~Assembler() {
    delete parser_;
    delete code_;
    delete symbol_table_;
    output_.close();
}

void Assembler::translate() {
    pass1();
    pass2();
}