#include "Trynd.h"
#include "Scanner.h"

#include <utility>

Scanner::Scanner(std::string source) : source(std::move(source)) {}

bool Scanner::isAtEnd() const {
    return current >= source.length();
}

char Scanner::advance() {
    return source[current++];
}

bool Scanner::isDigit(const char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(const char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        error(line, "Unterminated string!");
        return;
    }

    advance(); // for the closing quote

    // We -2 here to account for the end quote, and the +1 start
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance(); // consume the decimal
        while (isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER, std::stod(source.substr(start, current - start)));
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();

    const std::string text = source.substr(start, current - start);
    auto type = keywords.find(text);
    if (type == keywords.end()) {
        addToken(TokenType::IDENTIFIER);
    } else {
        addToken(type->second);
    }
}

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
    switch (const char c = advance()) {
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

        case '"':
            string();
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                error(line, "Unexpected character!");
            }
            break;
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

char Scanner::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}
