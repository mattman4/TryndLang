#ifndef TRYND_TOKEN_H
#define TRYND_TOKEN_H

#include <cmath>
#include <string>
#include <utility>
#include <variant>
#include <sstream>

#include "Literal.h"
#include "Callable.h"
#include "TokenType.h"

inline std::ostream& operator<<(std::ostream& os, const std::monostate&) {
    return os << "nil";
}

inline std::string literalToString(const Literal& literal) {
    // Special case for when a double is storing an integer
    if (std::holds_alternative<double>(literal) && std::get<double>(literal) == std::floor(std::get<double>(literal))) {
        return std::to_string(static_cast<int>(std::get<double>(literal)));
    }

    std::ostringstream oss;
    oss << std::boolalpha;
    std::visit([&oss](const auto& val) { oss << val; }, literal);
    return oss.str();
}

class Token {
public:
    const TokenType type;
    const Literal literal;
    const int line;
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
