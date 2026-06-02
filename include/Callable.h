#ifndef TRYND_CALLABLE_H
#define TRYND_CALLABLE_H

#include <vector>

#include "Literal.h"

class Interpreter;
class Callable {
public:
    virtual ~Callable() = default;
    virtual int arity() = 0;
    virtual Literal call(const Interpreter&, std::vector<Literal>) = 0;
};

#endif //TRYND_CALLABLE_H
