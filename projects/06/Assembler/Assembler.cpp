/**
    Main assembler program

    Modules
    - Parser: After parsing, access to each field is provided.
    - Code: Returns the binary code for the association symbol.
    - SymbolTable: Symbol table creation and symbol processing are performed.

    Process
    1. Initialization: Process declaration symbols.
    2. 1PASS: A symbol table is configured for a label(pseudo code).
    3. 2PASS: Translate each command into binary code.

    How to use
    prompt> Assember filePath
*/

#include <iostream>
#include <fstream>
#include <string>

class Parser {
private:
    std::ifstream input;
    std::string currentCommand;
    std::string nextCommand;

public:
    Parser(std::string path) : input(path) {
        if (input.fail()) {
            std::cout << "Can't find file" << std::endl;
            exit(100);
        }
        Initializer();
    }

    void deleteBlankOrComment(std::string& command) {
        int idx = 0;
        while (idx < command.size()) {
            if (command[idx] == ' ' || command[idx] == '\t') {
                command.erase(idx, (idx--)+1);
            } else if (idx > 0 && command[idx-1] == '/' && command[idx] == '/') {
                command.erase(idx-1, int(command.size()));
                break;
            }
            ++idx;
        }
    }

    std::string readCommand() {
        std::string buffer;
        while (!input.eof()) {
            std::getline(input, buffer);
            deleteBlankOrComment(buffer);
            if (int(buffer.size()) > 0) {
                return buffer;
            }
        }
        return "";
    }

    std::string getCurrentCommand() const {
        return currentCommand;
    }

    bool hasMoreCommands() const {
        if (int(nextCommand.size()) == 0) return false;
        else return true;
    }

    void moveNextCommand() {
        if (!hasMoreCommands()) {
            std::cout << "No more commands" << std::endl;
            return;
        }
        currentCommand = nextCommand;
        nextCommand = readCommand();
    }

    void Initializer() {
        currentCommand = readCommand();
        nextCommand = readCommand();
    }

    ~Parser() {
        input.close();
    }
};

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No input file" << std::endl;
        exit(100);
    } 
    Parser parser(argv[1]);
    while (parser.hasMoreCommands()) {
        std::cout << parser.getCurrentCommand() << std::endl;
        parser.moveNextCommand();
    }
    
    return 0;
}