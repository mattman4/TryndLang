#ifndef TRYND_SCANNER_H
#define TRYND_SCANNER_H

#include <string>
#include <vector>

#include "Token.h"

class Scanner {
  const std::string source;
  std::vector<Token> tokens;

  int start = 0;
  int current = 0;
  int line = 1;

  [[nodiscard]] bool isAtEnd() const;
  char advance();
  void scanToken();
  void addToken(TokenType type);
  void addToken(TokenType type, Literal literal);
  bool match(char);
  char peek() const;

public:
  explicit Scanner(std::string);

  std::vector<Token> scanTokens();
};

#endif //TRYND_SCANNER_H
