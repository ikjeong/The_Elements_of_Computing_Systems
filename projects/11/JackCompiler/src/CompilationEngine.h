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
#include "VMWriter.h"

class CompilationEngine {
private:
    JackTokenizer* jack_tokenizer_;
    SymbolTable* symbol_table_;
    VMWriter* vm_writer_;
    std::string file_name_;
    int indent_depth_;
    int while_label_index_;
    int if_label_index_;

    /* About setting module */
    void initialize(JackTokenizer* jackTokenizer, SymbolTable* symbolTable, VMWriter* vmWriter, const std::string& path);
    void advance(const std::string& expectedToken);
    void checkAndDefineIdentifier(const std::string& type, const VarKind varKind);
    void pushVariable(const VarKind varKind, const int index);
    void popToVariable(const VarKind varKind, const int index);

    /* Print about xml */
    void printIndent();
    void printStartTag(const std::string& tag);
    void printEndTag(const std::string& tag);
    void printTerminalNode(const TokenType tokenType, const std::string& token);
    std::string changeSymboltoXmlSymbol(const char& symbol) const;

    void printKeyword();
    void printSymbol();
    void printIdentifier(const VarKind varKind, const std::string& type = "", const int index = 0);
    void printIntegerConstant();
    void printStringConstant();

    /* Check token by using Tokenizer */
    bool checkKeyword(const std::string& keyword) const;
    bool checkKeyword(const std::vector<std::string>& keyword) const;
    bool checkSymbol(const char symbol) const;
    bool checkSymbol(const std::vector<char>& symbol) const;
    bool checkIdentifier(const VarKind varKind) const;
    bool checkIdentifier(const std::vector<VarKind>& varKind) const;
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
    void checkAndPrintIdentifier(const VarKind varKind);
    void checkAndPrintIdentifier(const std::vector<VarKind>& varKind);
    void checkAndPrintIntegerConstant();
    void checkAndPrintStringConstant();

    void checkAndPrintType();
    void checkAndCompileSubroutineCall();

    /* Compile XXX */
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList(const std::string& subroutineType);
    void compileSubroutineBody(const std::string& subroutineType, const std::string& subroutineName);
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    int compileExpressionList();

public:
    CompilationEngine();
    ~CompilationEngine();

    void compile(JackTokenizer* JackTokenizer, SymbolTable* symbolTable, VMWriter* vmWriter, const std::string& path);
};

#endif