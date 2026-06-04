#ifndef TRYND_STATEMENT_H
#define TRYND_STATEMENT_H

#include "Expression.h"

namespace Stmt {
    struct Block;
    struct Expression;
    struct Function;
    struct If;
    struct Print;
    struct Return;
    struct Var;
    struct While;

    using Stmt = std::variant<Block, Expression, Function, If, Print, Return, Var, While>;
    using StmtPtr = std::unique_ptr<Stmt>;

    struct Block {
        std::vector<StmtPtr> statements;

        explicit Block(std::vector<StmtPtr> statements) : statements(std::move(statements)) {}
    };

    struct Expression {
        Expr::ExprPtr expr;

        explicit Expression(Expr::ExprPtr expr) : expr(std::move(expr)) {}
    };

    struct Function {
        const Token name;
        std::vector<Token> params;
        std::shared_ptr<std::vector<StmtPtr>> body;

        Function(const Token& name, std::vector<Token> params, std::vector<StmtPtr> body) : name(name), params(std::move(params)), body(std::make_shared<std::vector<StmtPtr>>(std::move(body))) {}
    };

    struct If {
        Expr::ExprPtr condition;
        StmtPtr thenBranch;
        StmtPtr elseBranch;

        If(Expr::ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
    };

    struct Print {
        Expr::ExprPtr expr;

        explicit Print(Expr::ExprPtr expr) : expr(std::move(expr)) {}
    };

    struct Return {
        const Token keyword;
        Expr::ExprPtr value;

        Return(const Token& keyword, Expr::ExprPtr value) : keyword(keyword), value(std::move(value)) {}
    };

    struct Var {
        Token name;
        Expr::ExprPtr initialiser;

        explicit Var(const Token& name, Expr::ExprPtr initialiser) : name(name), initialiser(std::move(initialiser)) {}
    };

    struct While {
        Expr::ExprPtr condition;
        StmtPtr body;

        While(Expr::ExprPtr condition, StmtPtr body) : condition(std::move(condition)), body(std::move(body)) {}
    };
}

#endif //TRYND_STATEMENT_H
