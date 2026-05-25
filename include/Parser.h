#ifndef TRYND_PARSER_H
#define TRYND_PARSER_H

#include <vector>

#include "Expr.h"
#include "Token.h"

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    Expr::ExprPtr expression();
    Expr::ExprPtr equality();
    Expr::ExprPtr comparison();
    Expr::ExprPtr term();
    Expr::ExprPtr factor();
    Expr::ExprPtr unary();
    Expr::ExprPtr primary();

    bool match(std::initializer_list<TokenType>);
    bool check(TokenType);
    Token advance();
    Token consume(TokenType, const std::string&);

    bool isAtEnd();
    Token peek();
    Token previous();

    static ParseError error(const Token&, const std::string&);
    void synchronise();

public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    Expr::ExprPtr parse();
};

#endif //TRYND_PARSER_H
