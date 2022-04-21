/**
 * Compilation Engine Module(Class)
 * Perform actual compilation output.
 *
 * Interface
 * - compile: Proceed with the compilation and print it out.
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

    void printIndent();
    void printKeyword();
    void printSymbol();
    void printIntegerConstant();
    void printStringConstant();
    void printIdentifier();

    bool checkPrimitiveType();

    void checkAndPrintType();
    void checkAndPrintCommaAndVarName(const char& endSymbol);

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