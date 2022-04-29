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

/* =========== PUBLIC ============= */

CompilationEngine::CompilationEngine() {
}

CompilationEngine::~CompilationEngine() {
}