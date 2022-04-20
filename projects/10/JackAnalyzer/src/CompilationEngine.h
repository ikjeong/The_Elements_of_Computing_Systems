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
    std::ofstream& output_;

public:
    CompilationEngine();
    ~CompilationEngine();

    void compile(JackTokenizer* JackTokenizer, std::ofstream& output);
};

#endif