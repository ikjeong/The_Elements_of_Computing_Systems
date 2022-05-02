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

/**
 * Define Identifier.
 * Since var/argument are accessed first and then static/field variables are accessed, 
 * duplicate declarations between var/argument and static/field variables are possible.
 * However, duplicate declarations between static and field or 
 * duplicate declarations between static and field are not possible.
 */
void CompilationEngine::checkAndDefineIdentifier(const std::string& type, const VarKind varKind) {
    if (jack_tokenizer_->tokenType() != TokenType::IDENTIFIER) throw compile_exception("Expected Identifier for VarDec.", jack_tokenizer_->getCurrentTokenLineNumber());

    VarKind varKindInSymTable = symbol_table_->kindOf(jack_tokenizer_->getToken());
    if (varKindInSymTable == varKind) throw compile_exception("This variable is already defined.", jack_tokenizer_->getCurrentTokenLineNumber());
    else if (varKindInSymTable == VarKind::VAR && varKind == VarKind::ARGUMENT) throw compile_exception("This variable is already defined(var).", jack_tokenizer_->getCurrentTokenLineNumber());
    else if (varKindInSymTable == VarKind::ARGUMENT && varKind == VarKind::VAR) throw compile_exception("This variable is already defined(argument).", jack_tokenizer_->getCurrentTokenLineNumber());
    else if (varKindInSymTable == VarKind::FIELD && varKind == VarKind::STATIC) throw compile_exception("This variable is already defined(field).", jack_tokenizer_->getCurrentTokenLineNumber());
    else if (varKindInSymTable == VarKind::STATIC && varKind == VarKind::FIELD) throw compile_exception("This variable is already defined(static).", jack_tokenizer_->getCurrentTokenLineNumber());

    symbol_table_->define(jack_tokenizer_->getToken(), type, varKind);
}

/* =========== PUBLIC ============= */

CompilationEngine::CompilationEngine() {
}

CompilationEngine::~CompilationEngine() {
}