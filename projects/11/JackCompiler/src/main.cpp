/**
 * Main Jack Analyzer
 * v1: Implement parsing program
 *
 * Modules:
 * - JackCompiler: Top module to set up and load other modules.
 * - JackTokenizer: Tokenization module
 * - CompilationEngine: Recursive Down Parser
 * 
 * How to use
 * prompt> JackCompiler source
 * source is .jack file or directory which contains .jack files.
 * return .vm file.
 */

#include "JackCompiler.h"

int main(int argc, char* argv[]) {
    try {
        JackCompiler jackCompiler(argv[1]);
        jackCompiler.compile();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}