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
#include "SymbolTable.h"

class CompilationEngine {
private:
    JackTokenizer* jack_tokenizer_;
    SymbolTable* symbol_table_;
    std::ofstream* output_;
    int indent_depth_;

    /* About setting module */
    void initialize(JackTokenizer* jackTokenizer, SymbolTable* symbolTable, std::ofstream* output);
    void advance(const std::string& expectedToken);

    /* Print about xml */
    void printIndent();
    void printStartTag(const std::string& tag);
    void printEndTag(const std::string& tag);
    void printTerminalNode(const TokenType tokenType, const std::string& token);
    std::string changeSymboltoXmlSymbol(const char& symbol) const;

    void printKeyword();
    void printSymbol();
    void printIdentifier();
    void printIntegerConstant();
    void printStringConstant();

    /* Check token by using Tokenizer */
    bool checkKeyword(const std::string& keyword) const;
    bool checkKeyword(const std::vector<std::string>& keyword) const;
    bool checkSymbol(const char symbol) const;
    bool checkSymbol(const std::vector<char>& symbol) const;
    bool checkIdentifier(const std::string& expectedIdentifier) const;
    bool checkIdentifier(const std::vector<std::string>& expectedIdentifier) const;
    bool checkIntegerConstant() const;
    bool checkStringConstant() const;

    bool checkVarDecKeyword() const;
    bool checkSubroutineDecKeyword() const;
    bool checkKeywordConstant() const;
    bool checkPrimitiveType() const;
    bool checkType() const;
    bool checkTerm() const;
    bool checkOp() const;
    bool checkUnaryOp() const;

    /* Check and print token by using Tokenizer */
    void checkAndPrintKeyword(const std::string& keyword);
    void checkAndPrintKeyword(const std::vector<std::string>& keyword);
    void checkAndPrintSymbol(const char symbol);
    void checkAndPrintSymbol(const std::vector<char>& symbol);
    void checkAndPrintIdentifier(const std::string& expectedIdentifier);
    void checkAndPrintIdentifier(const std::vector<std::string>& expectedIdentifier);
    void checkAndPrintIntegerConstant();
    void checkAndPrintStringConstant();

    void checkAndPrintType();
    void checkAndPrintSubroutineCall();

    /* Compile XXX */
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

    void compile(JackTokenizer* JackTokenizer, SymbolTable* symbolTable, std::ofstream* output);
};

#endif