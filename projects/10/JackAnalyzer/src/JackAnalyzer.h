/**
    JackAnalyzer

    Interface: 
    - translate:
        tranlaste .jack to .xml file.

*/

#ifndef __JACK_ANALYZER_H__
#define __JACK_ANALYZER_H__

#include "Global.h"

class JackAnalyzer {
private:

public:
    JackAnalyzer(const std::string& path);
    ~JackAnalyzer();
    void translate();
};

#endif