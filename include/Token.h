#ifndef TRYND_TOKEN_H
#define TRYND_TOKEN_H

#include <string>
#include <utility>
#include <variant>
#include <sstream>

#include "TokenType.h"

using Literal = std::variant<std::monostate, std::string, int, double, bool>;

inline std::ostream& operator<<(std::ostream& os, const std::monostate&) {
    return os << "nil";
}

inline std::string literalToString(const Literal& literal) {
    std::ostringstream oss;
    std::visit([&oss](const auto& val) { oss << val; }, literal);
    return oss.str();
}

class Token {
    const TokenType type;
    const Literal literal;
    const int line;

public:
    const std::string lexeme;

    Token(const TokenType type, std::string lexeme, Literal literal, const int line) : type(type), literal(std::move(literal)), line(line), lexeme(std::move(lexeme)) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token(" << token.type << ", " << token.lexeme << ", ";
        std::visit([&os](const auto& val) { os << val; }, token.literal);
        std::cout << ")";
        return os;
    }
};

#endif //TRYND_TOKEN_H
