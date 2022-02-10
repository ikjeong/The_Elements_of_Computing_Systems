#ifndef __CODE_H__
#define __CODE_H__

#include "Global.h"

class Code {
private:
    const std::map<std::string, std::string> DEST = {
        {"",     "000"},
        {"M",    "001"},
        {"D",    "010"},
        {"MD",   "011"},
        {"A",    "100"},
        {"AM",   "101"},
        {"AD",   "110"},
        {"AMD",  "111"}
    };

    const std::map<std::string, std::string> COMP = {
        {"0",    "0101010"},
        {"1",    "0111111"},
        {"-1",   "0111010"},
        {"D",    "0001100"},
        {"A",    "0110000"},
        {"M",    "1110000"},
        {"!D",   "0001101"},
        {"!A",   "0110001"},
        {"!M",   "1110001"},
        {"-D",   "0001111"},
        {"-A",   "0110011"},
        {"-M",   "1110011"},
        {"D+1",  "0111111"},
        {"A+1",  "0110111"},
        {"M+1",  "1110111"},
        {"D-1",  "0001110"},
        {"A-1",  "0110010"},
        {"M-1",  "1110010"},
        {"D+A",  "0000010"},
        {"D+M",  "1000010"},
        {"D-A",  "0010011"},
        {"D-M",  "1010011"},
        {"A-D",  "0000111"},
        {"M-D",  "1000111"},
        {"D&A",  "0000000"},
        {"D&M",  "1000000"},
        {"D|A",  "0010101"},
        {"D|M",  "1010101"}
    };

    const std::map<std::string, std::string> JUMP = {
        {"",     "000"},
        {"JGT",  "001"},
        {"JEQ",  "010"},
        {"JGE",  "011"},
        {"JLT",  "100"},
        {"JNE",  "101"},
        {"JLE",  "110"},
        {"JMP",  "111"}
    };

public:
    Code() { }
    ~Code() { }

    std::bitset<15> address(const int& symbol) const {
        std::bitset<15> result(symbol);
        return result;
    }

    std::bitset<3> dest(const std::string& symbol) const {
        std::bitset<3> result(DEST.at(symbol));
        return result;
    }
    
    std::bitset<7> comp(const std::string& symbol) const {
        std::bitset<7> result(COMP.at(symbol));
        return result;
    }

    std::bitset<3> jump(const std::string& symbol) const {
        std::bitset<3> result(JUMP.at(symbol));
        return result;
    }
};

#endif