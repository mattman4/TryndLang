#ifndef TRYND_STATEMENT_H
#define TRYND_STATEMENT_H

#include "Expression.h"

namespace Stmt {
    struct Block;
    struct Expression;
    struct Print;
    struct Var;

    using Stmt = std::variant<Block, Expression, Print, Var>;
    using StmtPtr = std::unique_ptr<Stmt>;

    struct Block {
        std::vector<StmtPtr> statements;

        explicit Block(std::vector<StmtPtr> statements) : statements(std::move(statements)) {}
    };

    struct Expression {
        Expr::ExprPtr expr;

        explicit Expression(Expr::ExprPtr expr) : expr(std::move(expr)) {}
    };

    struct Print {
        Expr::ExprPtr expr;

        explicit Print(Expr::ExprPtr expr) : expr(std::move(expr)) {}
    };

    struct Var {
        Token name;
        Expr::ExprPtr initialiser;

        explicit Var(const Token& name, Expr::ExprPtr initialiser) : name(name), initialiser(std::move(initialiser)) {}
    };
}

#endif //TRYND_STATEMENT_H
