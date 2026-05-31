#include "Environment.h"
#include "Interpreter.h"

void Environment::define(const std::string& name, const Literal& value) {
    values[name] = value;
}

Literal Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}