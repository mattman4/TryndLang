#ifndef TRYND_INTERPRETER_H
#define TRYND_INTERPRETER_H

#include "Trynd.h"
#include "Expr.h"

class RuntimeError : public std::runtime_error {
public:
    const Token token;
    explicit RuntimeError(const Token& token, const std::string& message) : std::runtime_error(message), token(token) {}
};

class Interpreter {
    static bool isTruthy(const Literal&);

    static void checkNumberOperand(const Token&, const Literal&);
    static void checkNumberOperands(const Token&, const Literal&, const Literal&);
public:
    void interpret(Expr::Expr& expr);

    Literal evaluate(Expr::Expr& expr) {
        return std::visit([&](auto& e) { return evaluate(e); }, expr);
    }

    Literal evaluate(const Expr::Binary&);
    Literal evaluate(const Expr::Grouping&);
    static Literal evaluate(const Expr::LiteralExpr&);
    Literal evaluate(const Expr::Unary&);
};

#endif //TRYND_INTERPRETER_H
