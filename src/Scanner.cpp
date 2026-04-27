#include "Scanner.h"

#include <utility>

bool Scanner::isAtEnd() const {
    return current >= source.length();
}

char Scanner::advance() {
    return source[current++];
}

Scanner::Scanner(std::string source) : source(std::move(source)) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        // We're at the beginning of the next lexeme
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::EOF_, "", std::monostate(), line);
    return tokens;
}

void Scanner::scanToken() {
    switch (advance()) {
        case '(': addToken(TokenType::LEFT_BRACKET);  break;
        case ')': addToken(TokenType::RIGHT_BRACKET); break;
        case '{': addToken(TokenType::LEFT_BRACE);    break;
        case '}': addToken(TokenType::RIGHT_BRACE);   break;
        case ',': addToken(TokenType::COMMA);         break;
        case '.': addToken(TokenType::DOT);           break;
        case '-': addToken(TokenType::MINUS);         break;
        case '+': addToken(TokenType::PLUS);          break;
        case ';': addToken(TokenType::SEMI_COLON);    break;
        case '*': addToken(TokenType::STAR);          break;
        default:                                      break;
    }
}

void Scanner::addToken(const TokenType type) {
    addToken(type, std::monostate());
}

void Scanner::addToken(TokenType type, Literal literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, std::move(literal), line);
}
