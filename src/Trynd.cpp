#include <iostream>
#include <fstream>
#include <vector>
#include "Trynd.h"

#include "AstPrinter.h"
#include "Parser.h"
#include "Scanner.h"

bool hasError = false;

int main(const int argc, char* argv[]) {
    if(argc > 2) {
        std::cout << "Usage: trynd [<file>]" << std::endl;
        return EXIT_FAILURE;
    }

    /* AstPrinter printer;
    const std::string output = printer.print(Expr::Binary(
        std::make_unique<Expr::Expr>(Expr::Unary(
            Token(TokenType::MINUS, "-", std::monostate{}, 1),
            std::make_unique<Expr::Expr>(Expr::LiteralExpr(123))
        )),
        Token(TokenType::STAR, "*", std::monostate{}, 1),
        std::make_unique<Expr::Expr>(Expr::Grouping(
            std::make_unique<Expr::Expr>(Expr::LiteralExpr(45.67))
        ))
    ));
    std::cout << output << std::endl; */

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
    const std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    const Expr::ExprPtr expr = parser.parse();

    if (hasError) return;

    std::cout << AstPrinter().print(*expr);

    /*for (const Token& token : tokens) {
        std::cout << token << std::endl;
    }*/
}

namespace Error {
    void error(const int line, const std::string& message) {
        report(line, "", message);
    }

    void error(const Token& token, const std::string& message) {
        if (token.type == TokenType::EOF_) {
            report(token.line, "at end", message);
        } else {
            report(token.line, "at '" + token.lexeme + "'", message);
        }
    }
}

void report(const int line, const std::string& where, const std::string& message) {
    std::cout << "Error! Line " << line << " (" << where << "): " << message << std::endl;
    hasError = true;
}