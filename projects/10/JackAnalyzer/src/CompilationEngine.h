/**
 * Compilation Engine Module(Class)
 * Perform actual compilation output.
 *
 * Interface
 * - compile: Proceed with the compilation and print it out.
 * 
 * Caution
 * All member functions must already be pointed by the tokenizer module 
 * before processing for any xxx token. 
 * After processing the nonverbal node element, 
 * you must point to the last token that you process.
 * 
 * However, for functions that must be called even if there is no token element, 
 * the tokenizer module may not be pointing to the correct token. 
 * However, again, the tokenizer module points to a token that has not yet been processed.
 */

#ifndef __COMPILATION_ENGINE_H__
#define __COMPILATION_ENGINE_H__

#include "Global.h"
#include "JackTokenizer.h"

class CompilationEngine {
private:
    JackTokenizer* jack_tokenizer_;
    std::ofstream* output_;
    int indent_depth_;

    void initialize(JackTokenizer* jackTokenizer, std::ofstream* output);

    void advance(const std::string& expectedToken);

    void printStartTag(const std::string& tag);
    void printEndTag(const std::string& tag);
    void printIndent();
    void printKeyword();
    void printSymbol();
    void printIntegerConstant();
    void printStringConstant();
    void printIdentifier();

    std::string changeSymboltoXmlSymbol(const char& symbol) const;

    bool checkKeyword(const std::string& keyword) const;
    bool checkSymbol(const char& symbol) const;
    bool checkPrimitiveType() const;

    void checkAndPrintSymbol(const char& symbol);
    void checkAndPrintIdentifier(const std::string& expectedIdentifier);
    void checkAndPrintType();
    void checkAndPrintCommaAndVarName();
    void checkAndPrintTypeAndVarName();

    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileSubroutineBody();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();

public:
    CompilationEngine();
    ~CompilationEngine();

    void compile(JackTokenizer* JackTokenizer, std::ofstream* output);
};

#endif