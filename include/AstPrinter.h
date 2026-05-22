#ifndef TRYND_ASTPRINTER_H
#define TRYND_ASTPRINTER_H

#include "Expr.h"

class AstPrinter {
    template<typename... Args>
    std::string parenthesise(const std::string& name, const Args&... args) {
        std::string result = "(" + name;
        ((result += " " + print(*args)), ...);
        result += ")";
        return result;
    }
public:
    std::string print(Expr::Expr& expr) {
        return std::visit([&](auto& e) { return print(e); }, expr);
    }

    std::string print(const Expr::Binary& expr);
    std::string print(const Expr::Grouping& expr);
    static std::string print(const Expr::LiteralExpr& expr);
    std::string print(const Expr::Unary& expr);
};

#endif //TRYND_ASTPRINTER_H
