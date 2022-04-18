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

class JackAnalyzer {
private:
    std::unique_ptr<JackTokenizer> jackTokenizer;
    std::vector<std::string> paths_;

    void loadFilePaths(const std::string& path);
    bool isJackFile(const std::string& path) const;
    void analyzeFile(const std::string& path);
    std::string changeSymboltoXml(const char& symbol) const;

public:
    JackAnalyzer(const std::string& path);
    ~JackAnalyzer();
    void analyze();
};

#endif