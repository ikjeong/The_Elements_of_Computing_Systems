/**
    Main Jack Analyzer
    v1: Implement parsing program

    Modules:
    - JackAnalyzer: Top module to set up and load other modules.
    - JackTokenizer: Tokenization module
    - CompilationEngine: Recursive Down Parser

    How to use
    prompt> JackAnaylzer source
    source is .jack file or directory which contains .jack files.
    return .xml file.
*/

#include "JackAnalyzer.h"

int main(int argc, char* argv[]) {
    try {
        // JackAnalyzer에 입력 파일 경로를 제공하여 불러와 설정한다.
        JackAnalyzer jackAnalyzer(argv[1]);
        // JackAnalyzer의 구문 분석을 실행시킨다.
        jackAnalyzer.translate();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}