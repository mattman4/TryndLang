#ifndef TRYND_LITERAL_H
#define TRYND_LITERAL_H

#include <variant>
#include <string>

class Callable;
using Literal = std::variant<std::monostate, std::string, double, bool, Callable*>;

#endif //TRYND_LITERAL_H
