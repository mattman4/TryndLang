#include "Parser.h"
#include "Trynd.h"

Expr::ExprPtr Parser::parse() {
    try {
        return expression();
    } catch (const ParseError&) {
        return nullptr;
    }
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

ParseError Parser::error(const Token& token, const std::string& message) {
    Error::error(token, message);
    throw ParseError(message);
}

void Parser::synchronise() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMI_COLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUNC:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

bool Parser::match(const std::initializer_list<TokenType> types) {
    for (auto& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}


bool Parser::check(const TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}


Expr::ExprPtr Parser::expression() {
    return equality();
}

Expr::ExprPtr Parser::equality() {
    Expr::ExprPtr expr = comparison();

    while (match({TokenType::EQUAL_EQUAL, TokenType::EXCL_EQUAL})) {
        Token op = previous();
        Expr::ExprPtr right = comparison();
        expr = std::make_unique<Expr::Expr>(Expr::Binary(std::move(expr), op, std::move(right)));
    }

    return expr;
}

Expr::ExprPtr Parser::comparison() {
    Expr::ExprPtr expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        Expr::ExprPtr right = term();
        expr = std::make_unique<Expr::Expr>(Expr::Binary(std::move(expr), op, std::move(right)));
    }

    return expr;
}

Expr::ExprPtr Parser::term() {
    Expr::ExprPtr expr = factor();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        Expr::ExprPtr right = factor();
        expr = std::make_unique<Expr::Expr>(Expr::Binary(std::move(expr), op, std::move(right)));
    }

    return expr;
}

Expr::ExprPtr Parser::factor() {
    Expr::ExprPtr expr = unary();

    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        Expr::ExprPtr right = unary();
        expr = std::make_unique<Expr::Expr>(Expr::Binary(std::move(expr), op, std::move(right)));
    }

    return expr;
}

Expr::ExprPtr Parser::unary() {
    if (match({TokenType::EXCL, TokenType::MINUS})) {
        const Token op = previous();
        Expr::ExprPtr right = unary();
        return std::make_unique<Expr::Expr>(Expr::Unary(op, std::move(right)));
    }

    return primary();
}

Expr::ExprPtr Parser::primary() {
    if (match({TokenType::TRUE})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(true));
    if (match({TokenType::FALSE})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(false));
    if (match({TokenType::NIL})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(std::monostate()));
    if (match({TokenType::NUMBER, TokenType::STRING})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(previous().literal));
    if (match({TokenType::LEFT_BRACKET})) {
        Expr::ExprPtr expr = expression();
        consume(TokenType::RIGHT_BRACKET, "Expected ')' after expression.");
        return std::make_unique<Expr::Expr>(Expr::Grouping(std::move(expr)));
    }

    throw error(peek(), "Expected expression.");
}
