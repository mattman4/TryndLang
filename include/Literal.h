#ifndef TRYND_LITERAL_H
#define TRYND_LITERAL_H

#include <variant>
#include <string>
#include <memory>

class Callable;
using Literal = std::variant<std::monostate, std::string, double, bool, std::shared_ptr<Callable>>;

#endif //TRYND_LITERAL_H
