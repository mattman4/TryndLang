#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void runFile(const std::string&);

int main(int argc, char* argv[]) {

    if(argc > 2) {
        std::cout << "Usage: trynd [<file>]" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2) {
        runFile(argv[1]);
    } else {
        // Run prompt
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

    std::vector<char> chars(size);
    file.read(chars.data(), size);

    for(auto c : chars) std::cout << c;
}