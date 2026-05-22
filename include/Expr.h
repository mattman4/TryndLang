#ifndef TRYND_EXPR_H
#define TRYND_EXPR_H

#include <memory>

#include "Token.h"

namespace Expr {
    struct Binary;
    struct Grouping;
    struct LiteralExpr;
    struct Unary;

    using Expr = std::variant<Binary, Grouping, LiteralExpr, Unary>;
    using ExprPtr = std::unique_ptr<Expr>;

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
}

#endif //TRYND_EXPR_H
