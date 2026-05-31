#ifndef TRYND_EXPR_H
#define TRYND_EXPR_H

#include <memory>

#include "Token.h"

namespace Expr {
    struct Assign;
    struct Binary;
    struct Grouping;
    struct LiteralExpr;
    struct Unary;
    struct Variable;

    using Expr = std::variant<Assign, Binary, Grouping, LiteralExpr, Unary, Variable>;
    using ExprPtr = std::unique_ptr<Expr>;

    struct Assign {
        const Token name;
        ExprPtr value;

        Assign(const Token& name, ExprPtr value) : name(name), value(std::move(value)) {}
    };

    struct Binary {
        ExprPtr left;
        const Token op;
        ExprPtr right;

        Binary(ExprPtr left, const Token& op, ExprPtr right) : left(std::move(left)), op(op), right(std::move(right)) {}
    };

    struct Grouping {
        ExprPtr expr;

        explicit Grouping(ExprPtr expr) : expr(std::move(expr)) {}
    };

    struct LiteralExpr {
        const Literal literal;

        explicit LiteralExpr(const Literal& literal) : literal(literal) {}
    };

    struct Unary {
        const Token op;
        ExprPtr right;

        Unary(const Token& op, ExprPtr right) : op(op), right(std::move(right)) {}
    };

    struct Variable {
        const Token name;

        explicit Variable(const Token& name) : name(name) {}
    };
}

#endif //TRYND_EXPR_H
