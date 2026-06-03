#include "Function.h"

#include "Environment.h"
#include "Interpreter.h"

Literal Function::call(Interpreter& interpreter, const std::vector<Literal> arguments) {
    Environment environment(interpreter.globalEnvironment);

    for (int i = 0; i < declaration.params.size(); i++) {
        environment.define(declaration.params.at(i).lexeme, arguments.at(i));
    }

    interpreter.executeBlock(*declaration.body, &environment);

    return std::monostate();
}
