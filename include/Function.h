#ifndef TRYND_FUNCTION_H
#define TRYND_FUNCTION_H

#include "Callable.h"
#include "Statement.h"

class Function : public Callable {
    const Stmt::Function declaration;
public:
    explicit Function(const Stmt::Function& declaration) : declaration(declaration) {}

    int arity() override { return static_cast<int>(declaration.params.size()); }
    Literal call(Interpreter&, std::vector<Literal>) override;
};

#endif //TRYND_FUNCTION_H
