#ifndef TRYND_TRYND_H
#define TRYND_TRYND_H

#include <string>

void runFile(const std::string&);
void runPrompt();
void run(const std::string&);

void error(int, const std::string&);
void report(int, const std::string&, const std::string&);

#endif //TRYND_TRYND_H
