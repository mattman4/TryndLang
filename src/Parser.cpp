#include "Parser.h"

#include "Trynd.h"

std::vector<Stmt::StmtPtr> Parser::parse() {
    std::vector<Stmt::StmtPtr> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
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

Expr::ExprPtr Parser::finishCall(Expr::ExprPtr callee) {
    std::vector<Expr::ExprPtr> arguments;
    if (!check(TokenType::RIGHT_BRACKET)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Cannot have more than 255 arguments in a function.");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    const Token paren = consume(TokenType::RIGHT_BRACKET, "Expected ')' after function arguments.");

    return std::make_unique<Expr::Expr>(Expr::Call(std::move(callee), paren, std::move(arguments)));
}

Stmt::StmtPtr Parser::declaration() {
    try {
        if (match({TokenType::FUNC})) return function("function");
        if (match({TokenType::VAR})) return varDeclaration();
        return statement();
    } catch (const ParseError&) {
        synchronise();
        return nullptr;
    }
}

Stmt::StmtPtr Parser::statement() {
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_unique<Stmt::Stmt>(Stmt::Block(block()));
    return expressionStatement();
}

Stmt::StmtPtr Parser::ifStatement() {
    consume(TokenType::LEFT_BRACKET, "Expected '(' after if.");
    Expr::ExprPtr condition = expression();
    consume(TokenType::RIGHT_BRACKET, "Expected ')' after if condition.");

    Stmt::StmtPtr thenBranch = statement();
    Stmt::StmtPtr elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_unique<Stmt::Stmt>(Stmt::If(std::move(condition), std::move(thenBranch), std::move(elseBranch)));
}

Stmt::StmtPtr Parser::printStatement() {
    Expr::ExprPtr expr = expression();
    consume(TokenType::SEMI_COLON, "Expected ';' after expression.");
    return std::make_unique<Stmt::Stmt>(Stmt::Print(std::move(expr)));
}

Stmt::StmtPtr Parser::returnStatement() {
    const Token keyword = previous();
    Expr::ExprPtr value = nullptr;

    if (!check(TokenType::SEMI_COLON)) {
        value = expression();
    }

    consume(TokenType::SEMI_COLON, "Expected ';' after return.");
    return std::make_unique<Stmt::Stmt>(Stmt::Return(keyword, std::move(value)));
}

std::vector<Stmt::StmtPtr> Parser::block() {
    std::vector<Stmt::StmtPtr> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return statements;
}

Stmt::StmtPtr Parser::expressionStatement() {
    Expr::ExprPtr expr = expression();
    consume(TokenType::SEMI_COLON, "Expected ';' after expression.");
    return std::make_unique<Stmt::Stmt>(Stmt::Expression(std::move(expr)));
}

Stmt::StmtPtr Parser::function(const std::string& kind) {
    const Token name = consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
    consume(TokenType::LEFT_BRACKET, "Expected '(' after " + kind + " name.");

    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_BRACKET)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Cannot have more than 255 parameters in a function.");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_BRACKET, "Expected ')' after " + kind + " name.");

    consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body.");
    std::vector<Stmt::StmtPtr> body = block();

    return std::make_unique<Stmt::Stmt>(Stmt::Function(name, std::move(parameters), std::move(body)));
}

Stmt::StmtPtr Parser::varDeclaration() {
    const Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");

    Expr::ExprPtr initialiser = nullptr;
    if (match({TokenType::EQUAL})) {
        initialiser = expression();
    }

    consume(TokenType::SEMI_COLON, "Expected ';' after variable declaration.");
    return std::make_unique<Stmt::Stmt>(Stmt::Var(name, std::move(initialiser)));
}

Stmt::StmtPtr Parser::whileStatement() {
    consume(TokenType::LEFT_BRACKET, "Expected '(' after while.");
    Expr::ExprPtr condition = expression();
    consume(TokenType::RIGHT_BRACKET, "Expected ')' after while condition.");

    Stmt::StmtPtr body = statement();

    return std::make_unique<Stmt::Stmt>(Stmt::While(std::move(condition), std::move(body)));
}

Stmt::StmtPtr Parser::forStatement() {
    consume(TokenType::LEFT_BRACKET, "Expected '(' after for.");

    Stmt::StmtPtr initialiser;
    if (match({TokenType::SEMI_COLON})) {
        initialiser = nullptr;
    } else if (match({TokenType::VAR})) {
        initialiser = varDeclaration();
    } else {
        initialiser = expressionStatement();
    }

    Expr::ExprPtr condition = nullptr;
    if (!check(TokenType::SEMI_COLON)) {
        condition = expression();
    }
    consume(TokenType::SEMI_COLON, "Expected ';' after for condition.");

    Expr::ExprPtr increment = nullptr;
    if (!check(TokenType::RIGHT_BRACKET)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_BRACKET, "Expected ')' after for clauses.");

    Stmt::StmtPtr body = statement();

    if (increment != nullptr) {
        std::vector<Stmt::StmtPtr> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<Stmt::Stmt>(Stmt::Expression(std::move(increment))));
        body = std::make_unique<Stmt::Stmt>(Stmt::Block(std::move(statements)));
    }

    if (condition == nullptr) condition = std::make_unique<Expr::Expr>(Expr::LiteralExpr(true));
    body = std::make_unique<Stmt::Stmt>(Stmt::While(std::move(condition), std::move(body)));

    if (initialiser != nullptr) {
        std::vector<Stmt::StmtPtr> statements;
        statements.push_back(std::move(initialiser));
        statements.push_back(std::move(body));
        body = std::make_unique<Stmt::Stmt>(Stmt::Block(std::move(statements)));
    }

    return body;
}

Expr::ExprPtr Parser::expression() {
    return assignment();
}

Expr::ExprPtr Parser::assignment() {
    Expr::ExprPtr expr = or_();

    if (match({TokenType::EQUAL})) {
        const Token equals = previous();
        Expr::ExprPtr value = assignment();

        if (std::holds_alternative<Expr::Variable>(*expr)) {
            const Token name = std::get<Expr::Variable>(*expr).name;
            return std::make_unique<Expr::Expr>(Expr::Assign(name, std::move(value)));
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

Expr::ExprPtr Parser::or_() {
    Expr::ExprPtr expr = and_();

    while (match({TokenType::OR})) {
        Token op = previous();
        Expr::ExprPtr right = and_();
        expr = std::make_unique<Expr::Expr>(Expr::Logical(std::move(expr), op, std::move(right)));
    }

    return expr;
}

Expr::ExprPtr Parser::and_() {
    Expr::ExprPtr expr = equality();

    while (match({TokenType::AND})) {
        Token op = previous();
        Expr::ExprPtr right = equality();
        expr = std::make_unique<Expr::Expr>(Expr::Logical(std::move(expr), op, std::move(right)));
    }

    return expr;
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

    return call();
}

Expr::ExprPtr Parser::call() {
    Expr::ExprPtr expr = primary();

    for (;;) {
        if (match({TokenType::LEFT_BRACKET})) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }

    return expr;
}

Expr::ExprPtr Parser::primary() {
    if (match({TokenType::TRUE})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(true));
    if (match({TokenType::FALSE})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(false));
    if (match({TokenType::NIL})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(std::monostate()));
    if (match({TokenType::NUMBER, TokenType::STRING})) return std::make_unique<Expr::Expr>(Expr::LiteralExpr(previous().literal));
    if (match({TokenType::IDENTIFIER})) return std::make_unique<Expr::Expr>(Expr::Variable(previous()));
    if (match({TokenType::LEFT_BRACKET})) {
        Expr::ExprPtr expr = expression();
        consume(TokenType::RIGHT_BRACKET, "Expected ')' after expression.");
        return std::make_unique<Expr::Expr>(Expr::Grouping(std::move(expr)));
    }

    throw error(peek(), "Expected expression.");
}
