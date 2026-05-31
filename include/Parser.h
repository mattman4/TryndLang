#ifndef TRYND_PARSER_H
#define TRYND_PARSER_H

#include <vector>

#include "Expression.h"
#include "Token.h"
#include "Statement.h"

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
    std::vector<Token> tokens;
    int current = 0;

    // Statements
    Stmt::StmtPtr declaration();
    Stmt::StmtPtr statement();
    Stmt::StmtPtr printStatement();
    Stmt::StmtPtr expressionStatement();
    Stmt::StmtPtr varDeclaration();

    // Expressions
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
    std::vector<Stmt::StmtPtr> parse();
};

#endif //TRYND_PARSER_H
