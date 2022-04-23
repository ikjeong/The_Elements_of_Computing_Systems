/**
 * JackAnalyzer
 * 
 * Interface: 
 * - analyze: Parses all jack files in the program and saves them as .xml files.
 */

#ifndef __JACK_ANALYZER_H__
#define __JACK_ANALYZER_H__

#include "Global.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"

class JackAnalyzer {
private:
    std::unique_ptr<JackTokenizer> jack_tokenizer_;
    std::unique_ptr<CompilationEngine> compilation_engine_;
    std::vector<std::string> paths_;
    std::ofstream output_;
    
    void loadFilePaths(const std::string& path);
    bool isJackFile(const std::string& path) const;
    void analyzeFile(const std::string& path);
    void setOutputFile(std::string path);

public:
    JackAnalyzer(const std::string& path);
    ~JackAnalyzer();
    void analyze();
};

#endif