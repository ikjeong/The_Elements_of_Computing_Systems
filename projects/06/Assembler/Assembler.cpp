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

enum class CommandType { nothing = 0, address = 1, compute = 2, label = 3 };

class Parser {
private:
    std::ifstream input;
    std::string currentCommand;
    CommandType commandType;

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

    // parameter(command)'s size must be below 0
    void checkCommandType(std::string& command) {
        if (command.size() == 0) {
            commandType = CommandType::nothing;
        } else if (command[0] == '@') {
            commandType = CommandType::address;
        } else if (command[0] == '(' && command[command.size()-1] == ')') {
            commandType = CommandType::label;
        } else {
            commandType = CommandType::compute;
        }
    }

    std::string readCommand() {
        std::string buffer = "";
        while (!input.eof()) {
            std::getline(input, buffer);
            deleteBlankOrComment(buffer);
            if (int(buffer.size()) > 0) {
                return buffer;
            }
        }
        return buffer;
    }

    std::string getCurrentCommand() const {
        return currentCommand;
    }

    bool hasMoreCommands() const {
        if (int(currentCommand.size()) == 0) return false;
        else return true;
    }

    void advance() {
        if (!hasMoreCommands()) {
            std::cout << "No more commands" << std::endl;
            return;
        }
        currentCommand = readCommand();
        checkCommandType(currentCommand);
    }

    void Initializer() {
        currentCommand = readCommand();
        checkCommandType(currentCommand);
    }

    void resetCursor() {
        input.clear();
        input.seekg(0, std::ios::beg);
        Initializer();
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
        parser.advance();
    }
    parser.resetCursor();
    while (parser.hasMoreCommands()) {
        std::cout << parser.getCurrentCommand() << std::endl;
        parser.advance();
    }

    return 0;
}