#ifndef TRYND_TRYND_H
#define TRYND_TRYND_H

#include <string>

#include "Token.h"

class RuntimeError;

void runFile(const std::string&);
void runPrompt();
void run(const std::string&);

namespace Error {
    void error(int, const std::string&);
    void error(const Token&, const std::string&);
    void runtimeError(const RuntimeError&);
}
void report(int, const std::string&, const std::string&);

#endif //TRYND_TRYND_H
