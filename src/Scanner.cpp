#include "Trynd.h"
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
        case '(': addToken(TokenType::LEFT_BRACKET);      break;
        case ')': addToken(TokenType::RIGHT_BRACKET);     break;
        case '{': addToken(TokenType::LEFT_BRACE);        break;
        case '}': addToken(TokenType::RIGHT_BRACE);       break;
        case ',': addToken(TokenType::COMMA);             break;
        case '.': addToken(TokenType::DOT);               break;
        case '-': addToken(TokenType::MINUS);             break;
        case '+': addToken(TokenType::PLUS);              break;
        case ';': addToken(TokenType::SEMI_COLON);        break;
        case '*': addToken(TokenType::STAR);              break;

        case '!':
            addToken(match('=') ? TokenType::EXCL_EQUAL : TokenType::EXCL); break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if (match('/')) while (!isAtEnd() && peek() != '\n') advance(); // Keep consuming comment until end of line
            else addToken(TokenType::SLASH);

        // Whitespace
        case ' ':
        case '\r':
        case '\t':
            break;

        // New line
        case '\n':
            line++;
            break;

        default: error(line, "Unexpected character!"); break;
    }
}

void Scanner::addToken(const TokenType type) {
    addToken(type, std::monostate());
}

void Scanner::addToken(TokenType type, Literal literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, std::move(literal), line);
}

bool Scanner::match(const char expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}

char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source.at(current);
}
