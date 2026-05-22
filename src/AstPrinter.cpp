#include "AstPrinter.h"

std::string AstPrinter::print(const Expr::Binary& expr) {
    return parenthesise(expr.op.lexeme, expr.left, expr.right);
}

std::string AstPrinter::print(const Expr::Grouping& expr) {
    return parenthesise("group", expr.expr);
}

std::string AstPrinter::print(const Expr::LiteralExpr& expr) {
    if (std::holds_alternative<std::monostate>(expr.literal)) return "nil";
    return literalToString(expr.literal);
}

std::string AstPrinter::print(const Expr::Unary& expr) {
    return parenthesise(expr.op.lexeme, expr.right);
}
