/**
 * Implementation of CompilationEngine.h
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */

/**
 * Compile the entire class.
 */
void CompilationEngine::compileClass() {

}

/**
 * Compile static or field declarations.
 */
void CompilationEngine::compileClassVarDec() {

}

/**
 * Compiles an entire method, function, or constructor.
 */
void CompilationEngine::compileSubroutine() {

}

/**
 * Compile the parameter list (may be an empty list). It does not include '().
 */
void CompilationEngine::compileParameterList() {

}

/**
 * Compile the var declaration.
 */
void CompilationEngine::compileVarDec() {

}

/**
 * Compiles a series of statements. Does not include '{}'.
 */
void CompilationEngine::compileStatements() {

}

/**
 * Compile the do statement.
 */
void CompilationEngine::compileDo() {

}

/**
 * Compile the let statement.
 */
void CompilationEngine::compileLet() {

}

/**
 * compile the while statement
 */
void CompilationEngine::compileWhile() {

}

/**
 * Compile the return statement.
 */
void CompilationEngine::compileReturn() {

}

/**
 * Compile the if statement and the following else clause.
 */
void CompilationEngine::compileIf() {

}

/**
 * Compile an expression.
 */
void CompilationEngine::compileExpression() {

}

/**
 * Compile an term.
 */
void CompilationEngine::compileTerm() {

}

/**
 * Compiles a comma-separated list of expressions (which can be an empty list)
 */
void CompilationEngine::compileExpressionList() {

}

/* =========== PUBLIC ============= */

CompilationEngine::CompilationEngine() {
}

CompilationEngine::~CompilationEngine() {
}

/**
 * Set up the tokenization module and output file, and start the compilation output.
 * @param jackTokenizer The module that has completed tokenization must be delivered.
 * @param output Specifies the file to be compiled output. It must be in the .xml file format.
 */
void CompilationEngine::compile(JackTokenizer* jackTokenizer, std::ofstream* output) {
    jack_tokenizer_ = jackTokenizer;
    output_ = output;
    while (jack_tokenizer_->hasMoreTokens()) {
        jack_tokenizer->advance();
        if (jack_tokenizer_->tokenType() == TokenType::KEYWORD && 
            jack_tokenizer->keyword() == "class") {
            compileClass();
        } else {
            throw translate_exception("The first syntax must be class.");
        }
    }
}