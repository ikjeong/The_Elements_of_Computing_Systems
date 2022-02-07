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
enum class Error { fileOpen = 100, functionCall = 200};

class Parser {
private:
    std::ifstream input;
    std::string currentCommand;
    CommandType type;


public:
    int currentLineNumber;
    Parser(std::string path) : input(path), currentLineNumber(0) {
        if (input.fail()) {
            std::cout << "Can't find file" << std::endl;
            std::exit(int(Error::fileOpen));
        }
        Initializer();
    }

    void deleteBlankOrComment(std::string& command) {
        int pos = command.find("//");
        if (pos != (int)std::string::npos) command.erase(pos,std::string::npos);

        int idx = 0;
        while (idx < command.size()) {
            if (command[idx] == ' ' || command[idx] == '\t') {
                command.erase(idx--, 1);
            }
            ++idx;
        }
    }

    void checkCommandType(std::string& command) {
        if (command.size() == 0) {
            type = CommandType::nothing;
        } else if (command[0] == '@') {
            type = CommandType::address;
            // @{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*
        } else if (command[0] == '(' && command[command.size()-1] == ')') {
            type = CommandType::label;
            // [(]{1}[a-zA-Z_.$:]+[a-zA-Z_.&:0-9]*[)]{1}
        } else {
            type = CommandType::compute;
        }
    }

    std::string readCommand() {
        std::string buffer = "";
        while (!input.eof()) {
            std::getline(input, buffer);
            ++currentLineNumber;
            deleteBlankOrComment(buffer);
            if (int(buffer.size()) > 0) return buffer;
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
        if (!hasMoreCommands()) return;
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

    CommandType commandType() const {
        return type;
    }

    std::string symbol() const {
        if (type != CommandType::address && type != CommandType::label) {
            std::cout << "Incorrect function call." << std::endl;
            std::exit(int(Error::functionCall));
        }
        std::string result = currentCommand.substr(1, currentCommand.size()-1);
        if (result[result.size()-1] == ')') result.pop_back();
        return result;
    }

    std::string dest() const {
        if (type != CommandType::compute) {
            std::cout << "Incorrect function call." << std::endl;
            std::exit(int(Error::functionCall));
        }
        std::string::size_type destPos = currentCommand.find("=");
        std::string result = "";
        if (destPos == std::string::npos) return result;
        result = currentCommand.substr(0, destPos);
        return result;
    }

    std::string comp() const {
        if (type != CommandType::compute) {
            std::cout << "Incorrect function call." << std::endl;
            std::exit(int(Error::functionCall));
        }
        std::string::size_type destPos = currentCommand.find("=");
        std::string::size_type jumpPos = currentCommand.find(";");
        std::string result = currentCommand;
        if (destPos != std::string::npos) result.erase(0, destPos+1);
        if (jumpPos != std::string::npos) result.erase(jumpPos, std::string::npos);
        return result;
    }

    std::string jump() const {
        if (type != CommandType::compute) {
            std::cout << "Incorrect function call." << std::endl;
            std::exit(int(Error::functionCall));
        }
        std::string::size_type jumpPos = currentCommand.find(";");
        std::string result = "";
        if (jumpPos == std::string::npos) return result;
        result = currentCommand.substr(jumpPos+1, std::string::npos);
        return result;
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
        std::cout << parser.currentLineNumber << ": ";
        std::cout << parser.getCurrentCommand() << std::endl;
        if (parser.commandType() == CommandType::label || parser.commandType() == CommandType::address) {
            std::cout << parser.symbol() << std::endl;
        } else if (parser.commandType() == CommandType::compute) {
            std::cout << parser.dest() << std::endl;
            std::cout << parser.comp() << std::endl;
            std::cout << parser.jump() << std::endl;
        }
        parser.advance();
    }

    return 0;
}