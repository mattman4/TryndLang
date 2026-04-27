#ifndef TRYND_TOKEN_H
#define TRYND_TOKEN_H

#include <string>

#include "TokenType.h"

class Token {
    const TokenType type;
    const std::string lexeme;
    const int line;

public:
    Token(const TokenType type, const std::string& lexeme, const int line) : type(type), lexeme(lexeme), line(line) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token(" << token.type << " " << token.lexeme << ")";
        return os;
    }
};

#endif //TRYND_TOKEN_H
