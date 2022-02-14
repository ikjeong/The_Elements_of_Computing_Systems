/**
    Implementation of Assembler.h
*/

#include "Assembler.h"

/* =========== PRIVATE ============= */

bool Assembler::isASMFile(const std::string path) const {
    return path.find(".asm") != std::string::npos;
}

void Assembler::writePart(SymbolType type) {
    std::string (Parser::*symbol)() const;

    if (type == SymbolType::address) {
        symbol = &Parser::symbol;
    } else if (type == SymbolType::comp) {
        symbol = &Parser::comp;
    } else if (type == SymbolType::dest) {
        symbol = &Parser::dest;
    } else if (type == SymbolType::jump) {
        symbol = &Parser::jump;
    } else {
        throw functionCallException("Incorrect Symbol Type (Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    }

    if (code_->canTranslateToBinary((parser_->*symbol)(), type))
        if (type == SymbolType::address)   output_ << code_->address(std::stoi((parser_->*symbol)()));
        else if (type == SymbolType::comp) output_ << code_->comp((parser_->*symbol)());
        else if (type == SymbolType::dest) output_ << code_->dest((parser_->*symbol)());
        else if (type == SymbolType::jump) output_ << code_->jump((parser_->*symbol)());
        else throw functionCallException("Incorrect Symbol Type (Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    else if (symbol_table_->contains((parser_->*symbol)()))
        output_ << code_->address(symbol_table_->GetAddress((parser_->*symbol)()));
    else
        output_ << code_->address(symbol_table_->addVariable((parser_->*symbol)()));
}

void Assembler::writeACommand() {
    try {
        output_ << "0";
        writePart(SymbolType::address);
    } catch (functionCallException& e) {
        throw e;
    } catch (std::exception& e) {
        throw translateException("ADDRESS: " + parser_->symbol() + "(Parse line: " + std::to_string(parser_->getCurrentCommandFileLine()) + ")");
    }
}

void Assembler::writeCCommand() {
    try {
        output_ << "111";
        writePart(SymbolType::comp);
        writePart(SymbolType::dest);
        writePart(SymbolType::jump);
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