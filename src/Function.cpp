#include "Function.h"

#include "Environment.h"
#include "Interpreter.h"

Literal Function::call(Interpreter& interpreter, const std::vector<Literal> arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);

    for (int i = 0; i < declaration.params.size(); i++) {
        environment->define(declaration.params.at(i).lexeme, arguments.at(i));
    }

    try {
        interpreter.executeBlock(*declaration.body, environment);
    } catch (const Return& returnValue) {
        return returnValue.value;
    }

    return std::monostate();
}
