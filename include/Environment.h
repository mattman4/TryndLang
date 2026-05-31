#ifndef TRYND_ENVIRONMENT_H
#define TRYND_ENVIRONMENT_H

#include <unordered_map>

#include "Token.h"

class Environment {
    Environment* enclosing;
    std::unordered_map<std::string, Literal> values;
public:
    Environment() : enclosing(nullptr) {};
    explicit Environment(Environment* enclosing) : enclosing(enclosing) {}

    void define(const std::string&, const Literal&);
    Literal get(const Token&);
    void assign(const Token&, const Literal&);
};

#endif //TRYND_ENVIRONMENT_H
