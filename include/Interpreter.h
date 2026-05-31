#ifndef TRYND_INTERPRETER_H
#define TRYND_INTERPRETER_H

#include <vector>

#include "Environment.h"
#include "Trynd.h"
#include "Expression.h"
#include "Statement.h"

class RuntimeError : public std::runtime_error {
public:
    const Token token;
    explicit RuntimeError(const Token& token, const std::string& message) : std::runtime_error(message), token(token) {}
};

class Interpreter {
    Environment* environment;
    Environment globalEnvironment;

    static bool isTruthy(const Literal&);

    static void checkNumberOperand(const Token&, const Literal&);
    static void checkNumberOperands(const Token&, const Literal&, const Literal&);

    void executeBlock(const std::vector<Stmt::StmtPtr>&, Environment*);
public:
    Interpreter() : environment(&globalEnvironment) {}

    void interpret(const std::vector<Stmt::StmtPtr>&);

    Literal evaluate(const Expr::Expr& expr) {
        return std::visit([&](auto& e) { return evaluate(e); }, expr);
    }

    void execute(const Stmt::Stmt& stmt) {
        std::visit([&](auto& e) { execute(e); }, stmt);
    }

    // Statements
    void execute(const Stmt::Block&);
    void execute(const Stmt::Expression&);
    void execute(const Stmt::Print&);
    void execute(const Stmt::Var&);

    // Expressions
    Literal evaluate(const Expr::Binary&);
    Literal evaluate(const Expr::Grouping&);
    static Literal evaluate(const Expr::LiteralExpr&);
    Literal evaluate(const Expr::Unary&);
    Literal evaluate(const Expr::Variable&);
    Literal evaluate(const Expr::Assign&);
};

#endif //TRYND_INTERPRETER_H
