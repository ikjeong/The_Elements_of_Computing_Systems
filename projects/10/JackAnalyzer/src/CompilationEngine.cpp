/**
 * Implementation of CompilationEngine.h
 */

#include "CompilationEngine.h"

/* =========== PRIVATE ============= */
/**
 * All compileXXX() member functions must have
 * the tokenization module pointing to the token
 * that fits XXX before being called.
 */

void CompilationEngine::printIndent() {
    for (int i = 0; i < indent_depth_ * 2; ++i)
        *output_ << ' ';
}

void CompilationEngine::printKeyword() {
    printIndent();
    *output_ << "<keyword> " << jack_tokenizer_->keyword() << " </keyword>";
    *output_ << std::endl;
}

void CompilationEngine::printSymbol() {
    printIndent();
    *output_ << "<symbol> " << jack_tokenizer_->symbol() << " </symbol>";
    *output_ << std::endl;
}

void CompilationEngine::printIntegerConstant() {
    printIndent();
    *output_ << "<integerConstant> " << jack_tokenizer_->intVal() << " </integerConstant>";
    *output_ << std::endl;
}

void CompilationEngine::printStringConstant() {
    printIndent();
    *output_ << "<stringConstant> " << jack_tokenizer_->stringVal() << " </stringConstant>";
    *output_ << std::endl;
}

void CompilationEngine::printIdentifier() {
    printIndent();
    *output_ << "<identifier> " << jack_tokenizer_->identifier() << " </identifier>";
    *output_ << std::endl;
}

/**
 * Compile the entire class.
 */
void CompilationEngine::compileClass() {
    ++indent_depth_;
    /* 'class' className '{' classVarDec* subroutineDec* '}' */
    *output_ << "<class>" << std::endl;

    printKeyword(); // It must be 'class'
    jack_tokenizer_->advance();

    if (jack_tokenizer_->tokenType() == TokenType::IDENTIFIER) printIdentifier();
    else throw analyze_exception("Required identifier(className)");
    jack_tokenizer_->advance();

    if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
        jack_tokenizer_->symbol() == '{') printSymbol();
    else throw analyze_exception("Required Symbol('{')");
    jack_tokenizer_->advance();

    while (true) {
        if (jack_tokenizer_->tokenType() == TokenType::SYMBOL && 
           jack_tokenizer_->symbol() == '}') break;
        
        if (jack_tokenizer_->tokenType() != TokenType::KEYWORD)
            throw analyze_exception("Required Symbol(classVarDec or subroutineDec)");

        if (jack_tokenizer_->keyword() == "static") compileClassVarDec();
        else if (jack_tokenizer_->keyword() == "field") compileClassVarDec();
        else if (jack_tokenizer_->keyword() == "constructor") compileSubroutine();
        else if (jack_tokenizer_->keyword() == "function") compileSubroutine();
        else if (jack_tokenizer_->keyword() == "method") compileSubroutine();
        else throw analyze_exception("Required Symbol(classVarDec or subroutineDec)");

        jack_tokenizer_->advance();
    }

    printSymbol(); // It must be '}'
    if (jack_tokenizer_->hasMoreTokens())
        jack_tokenizer_->advance();

    *output_ << "</class>" << std::endl;
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
    indent_depth_ = 0;
    while (jack_tokenizer_->hasMoreTokens()) {
        jack_tokenizer_->advance();
        if (jack_tokenizer_->tokenType() == TokenType::KEYWORD && 
            jack_tokenizer_->keyword() == "class") {
            compileClass();
        } else {
            throw analyze_exception("The first syntax must be class");
        }
    }
}