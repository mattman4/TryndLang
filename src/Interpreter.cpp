#include "Interpreter.h"

bool Interpreter::isTruthy(const Literal& literal) {
    if (std::holds_alternative<std::monostate>(literal)) return false;
    if (std::holds_alternative<bool>(literal)) return std::get<bool>(literal);
    return true;
}

void Interpreter::checkNumberOperand(const Token& token, const Literal& literal) {
    if (std::holds_alternative<double>(literal)) return;
    throw RuntimeError(token, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& token, const Literal& left, const Literal& right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(token, "Operands must be numbers.");
}

void Interpreter::executeBlock(const std::vector<Stmt::StmtPtr>& statements, Environment* innerEnvironment) {
    Environment* previous = this->environment;
    try {
        this->environment = innerEnvironment;

        for (const Stmt::StmtPtr& stmt : statements) execute(*stmt);
    } catch (...) {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}

void Interpreter::interpret(const std::vector<Stmt::StmtPtr>& statements) {
    try {
        for (const Stmt::StmtPtr& stmt : statements) {
            execute(*stmt);
        }
    } catch (RuntimeError& e) {
        Error::runtimeError(e);
    }
}

void Interpreter::execute(const Stmt::Block& stmt) {
    Environment newEnvironment(environment);
    executeBlock(stmt.statements, &newEnvironment);
}

void Interpreter::execute(const Stmt::Expression& stmt) {
    evaluate(*stmt.expr);
}

void Interpreter::execute(const Stmt::Print& stmt) {
    const Literal literal = evaluate(*stmt.expr);
    std::cout << literalToString(literal) << std::endl;
}

void Interpreter::execute(const Stmt::Var& stmt) {
    Literal value = std::monostate();

    if (stmt.initialiser != nullptr) {
        value = evaluate(*stmt.initialiser);
    }

    environment->define(stmt.name.lexeme, value);
}

Literal Interpreter::evaluate(const Expr::Binary& expr) {
    const Literal left = evaluate(*expr.left);
    const Literal right = evaluate(*expr.right);

    switch (expr.op.type) {

        case TokenType::MINUS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) - std::get<double>(left);
        case TokenType::PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            }
            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            throw RuntimeError(expr.op, "Operands must be both numbers or both strings.");
        case TokenType::SLASH:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) / std::get<double>(left);
        case TokenType::STAR:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) * std::get<double>(left);

        case TokenType::GREATER:
            checkNumberOperands(expr.op, left, right);
            return left > right;
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return left >= right;
        case TokenType::LESS:
            checkNumberOperands(expr.op, left, right);
            return left < right;
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return left <= right;

        case TokenType::EQUAL_EQUAL:
            return left == right;
        case TokenType::EXCL_EQUAL:
            return left != right;

        default:
            break;
    }

    return false;
}

Literal Interpreter::evaluate(const Expr::Grouping& expr) {
    return evaluate(*expr.expr);
}

Literal Interpreter::evaluate(const Expr::LiteralExpr& expr) {
    return expr.literal;
}

Literal Interpreter::evaluate(const Expr::Unary& expr) {
    const Literal right = evaluate(*expr.right);

    switch (expr.op.type) {
        case TokenType::MINUS:
            checkNumberOperand(expr.op, right);
            return -std::get<double>(right);
        case TokenType::EXCL:
            return !isTruthy(right);
        default:
            break;
    }

    return std::monostate();
}

Literal Interpreter::evaluate(const Expr::Variable& expr) {
    return environment->get(expr.name);
}

Literal Interpreter::evaluate(const Expr::Assign& expr) {
    const Literal value = evaluate(*expr.value);
    environment->assign(expr.name, value);
    return value;
}