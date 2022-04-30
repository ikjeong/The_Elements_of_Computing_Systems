/**
 * JackCompiler
 * 
 * Interface: 
 * - compile: Compile the jack file and output the vm file.
 */

#ifndef __JACK_COMPILER_H__
#define __JACK_COMPILER_H__

#include "Global.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "SymbolTable.h"

class JackCompiler {
private:
    std::unique_ptr<JackTokenizer> jack_tokenizer_;
    std::unique_ptr<SymbolTable> symbol_table_;
    std::unique_ptr<CompilationEngine> compilation_engine_;
    
    std::vector<std::string> paths_;
    std::string root_path_;
    
    /* It need to be replaced. */
    std::ofstream output_;
    void setOutputFile(std::string path);

    void loadFilePaths(const std::string& path);
    bool isJackFile(const std::string& path) const;
    void compileFile(const std::string& path);

public:
    JackCompiler(const std::string& path);
    ~JackCompiler();
    void compile();
};

#endif