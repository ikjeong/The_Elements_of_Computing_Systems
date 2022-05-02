/**
 * Implementation of CompilationEngine.h
 * Implement the member functions associated with the module setup.
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

/**
 * Set up the tokenization module and output file.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param symbolTable Module that manages symbol tables.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::initialize(JackTokenizer* jackTokenizer, SymbolTable* symbolTable, std::ofstream* output) {
    jack_tokenizer_ = jackTokenizer;
    symbol_table_ = symbolTable;
    output_ = output;
    indent_depth_ = 0;
}

/**
 * Advance Tokenizer.
 * @param expectedToken when fail to advance, throw exception with message includes expected token.
 */
void CompilationEngine::advance(const std::string& expectedToken) {
    if (jack_tokenizer_->hasMoreTokens()) jack_tokenizer_->advance();
    else throw compile_exception("Next token does not exist. Compiler expects " + expectedToken + std::string("."));
}

/* =========== PUBLIC ============= */

CompilationEngine::CompilationEngine() {
}

CompilationEngine::~CompilationEngine() {
}