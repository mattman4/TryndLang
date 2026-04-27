#ifndef TRYND_TOKENS_H
#define TRYND_TOKENS_H

#include <iostream>

enum class TokenType {
    // Single character (){},.-+;/*
    LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMI_COLON, SLASH, STAR,

    // 1-2 character ! != = == > >= < <=
    EXCL, EXCL_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, OR, TRUE, FALSE, IF, ELSE,
    VAR, FUNC, CLASS, FOR, WHILE,
    NIL, THIS, SUPER, RETURN, PRINT,

    _EOF
};

inline std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    switch(type) {
        case TokenType::LEFT_BRACKET:    os << "LEFT_BRACKET";    break;
        case TokenType::RIGHT_BRACKET:   os << "RIGHT_BRACKET";   break;
        case TokenType::LEFT_BRACE:      os << "LEFT_BRACE";      break;
        case TokenType::RIGHT_BRACE:     os << "RIGHT_BRACE";     break;
        case TokenType::COMMA:           os << "COMMA";           break;
        case TokenType::DOT:             os << "DOT";             break;
        case TokenType::MINUS:           os << "MINUS";           break;
        case TokenType::PLUS:            os << "PLUS";            break;
        case TokenType::SEMI_COLON:      os << "SEMI_COLON";      break;
        case TokenType::SLASH:           os << "SLASH";           break;
        case TokenType::STAR:            os << "STAR";            break;
        case TokenType::EXCL:            os << "EXCL";            break;
        case TokenType::EXCL_EQUAL:      os << "EXCL_EQUAL";      break;
        case TokenType::EQUAL:           os << "EQUAL";           break;
        case TokenType::EQUAL_EQUAL:     os << "EQUAL_EQUAL";     break;
        case TokenType::GREATER:         os << "GREATER";         break;
        case TokenType::GREATER_EQUAL:   os << "GREATER_EQUAL";   break;
        case TokenType::LESS:            os << "LESS";            break;
        case TokenType::LESS_EQUAL:      os << "LESS_EQUAL";      break;
        case TokenType::IDENTIFIER:      os << "IDENTIFIER";      break;
        case TokenType::STRING:          os << "STRING";          break;
        case TokenType::NUMBER:          os << "NUMBER";          break;
        case TokenType::AND:             os << "AND";             break;
        case TokenType::OR:              os << "OR";              break;
        case TokenType::TRUE:            os << "TRUE";            break;
        case TokenType::FALSE:           os << "FALSE";           break;
        case TokenType::IF:              os << "IF";              break;
        case TokenType::ELSE:            os << "ELSE";            break;
        case TokenType::VAR:             os << "VAR";             break;
        case TokenType::FUNC:            os << "FUNC";            break;
        case TokenType::CLASS:           os << "CLASS";           break;
        case TokenType::FOR:             os << "FOR";             break;
        case TokenType::WHILE:           os << "WHILE";           break;
        case TokenType::NIL:             os << "NIL";             break;
        case TokenType::THIS:            os << "THIS";            break;
        case TokenType::SUPER:           os << "SUPER";           break;
        case TokenType::RETURN:          os << "RETURN";          break;
        case TokenType::PRINT:           os << "PRINT";           break;
        case TokenType::_EOF:            os << "_EOF";            break;
        default: break;
    }
    return os;
}

#endif //TRYND_TOKENS_H
