/**
    Implementation of VMtranslator.h
*/

#include "VMtranslator.h"

/* =========== PRIVATE ============= */

void VMtranslator::loadFilePaths(const std::string& path) {
    if (std::filesystem::is_directory(path)){
        for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_directory(entry.path())) continue;
            if (!isVMFile(entry.path())) continue;
            paths_.push_back(entry.path());
        }
    } else {
        if (!isVMFile(path)) return;
        paths_.push_back(path);
    }
}

void VMtranslator::translateFile(const std::string& path) {
    parser_->setNewFile(path);
    code_writer_->setFileName(path);
    while (parser_->hasMoreCommands()) {
        parser_->advance();
        if (parser_->commandType() == CommandType::C_ARITHMETIC) code_writer_->writeArithmetic(parser_->arg1());
        else if (parser_->commandType() == CommandType::C_PUSH) code_writer_->writePushPop(parser_->commandType(), parser_->arg1(), parser_->arg2());
        else if (parser_->commandType() == CommandType::C_POP) code_writer_->writePushPop(parser_->commandType(), parser_->arg1(), parser_->arg2());
        else throw translate_exception("It's an command type that can't be translated.");
    }
}

bool VMtranslator::isVMFile(const std::string& path) const {
    return path.find(".vm") != std::string::npos;
}

/* =========== PUBLIC ============= */

VMtranslator::VMtranslator(const std::string& path) {
    parser_ = new Parser();
    code_writer_ = new CodeWriter(path);
    loadFilePaths(path);
}

VMtranslator::~VMtranslator() {
    delete parser_;
    delete code_writer_;
}

void VMtranslator::translate() {
    if (paths_.empty()) throw file_exception("There is no .vm file.");
    for (auto path : paths_) {
        translateFile(path);
    }
}