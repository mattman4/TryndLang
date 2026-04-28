#include <iostream>
#include <fstream>
#include <vector>
#include "Trynd.h"

#include "Scanner.h"

bool hasError = false;

int main(const int argc, char* argv[]) {
    if(argc > 2) {
        std::cout << "Usage: trynd [<file>]" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return EXIT_SUCCESS;
}

void runFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);

    if(!file) {
        std::cout << "Error: file '" << path << "' not found" << std::endl; 
        std::exit(EXIT_FAILURE);
    }

    // Find out size
    file.seekg(0, std::ios::end);
    const std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string source(size, '\0');
    file.read(source.data(), size);

    run(source);

    if (hasError) std::exit(EXIT_FAILURE);
}

void runPrompt() {
    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        run(line);
        hasError = false;
    }
}

void run(const std::string& source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    for (const Token& token : tokens) {
        std::cout << token << std::endl;
    }
}

void error(const int line, const std::string& message) {
    report(line, "", message);
}

void report(const int line, const std::string& where, const std::string& message) {
    std::cout << "Error! Line " << line << " (" << where << "): " << message << std::endl;
    hasError = true;
}