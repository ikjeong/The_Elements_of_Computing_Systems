/**
 * Main Jack Analyzer
 * v1: Implement parsing program
 *
 * Modules:
 * - JackAnalyzer: Top module to set up and load other modules.
 * - JackTokenizer: Tokenization module
 * - CompilationEngine: Recursive Down Parser
 * 
 * How to use
 * prompt> JackAnaylzer source
 * source is .jack file or directory which contains .jack files.
 * return .xml file.
 */

#include "JackAnalyzer.h"

int main(int argc, char* argv[]) {
    try {
        JackAnalyzer jackAnalyzer(argv[1]);
        jackAnalyzer.analyze();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}