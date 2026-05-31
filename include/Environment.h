#ifndef TRYND_ENVIRONMENT_H
#define TRYND_ENVIRONMENT_H

#include <unordered_map>

#include "Token.h"

class Environment {
    std::unordered_map<std::string, Literal> values;
public:
    void define(const std::string&, const Literal&);
    Literal get(const Token&);
};

#endif //TRYND_ENVIRONMENT_H
