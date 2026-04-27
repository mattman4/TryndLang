#ifndef TRYND_TOKEN_H
#define TRYND_TOKEN_H

#include <string>
#include <utility>
#include <variant>

#include "TokenType.h"

using Literal = std::variant<std::monostate, std::string, int, double, bool>;

inline std::ostream& operator<<(std::ostream& os, const std::monostate&) {
    return os << "nil";
}

class Token {
    const TokenType type;
    const std::string lexeme;
    const Literal literal;
    const int line;

public:
    Token(const TokenType type, std::string lexeme, Literal literal, const int line) : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token(" << token.type << ", " << token.lexeme << ", ";
        std::visit([&os](const auto& val) { os << val; }, token.literal);
        std::cout << ")";
        return os;
    }
};

#endif //TRYND_TOKEN_H
