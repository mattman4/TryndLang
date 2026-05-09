#ifndef TRYND_SCANNER_H
#define TRYND_SCANNER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"

class Scanner {
  inline static const std::unordered_map<std::string, TokenType> keywords = {
    {"and",    TokenType::AND},
    {"or",     TokenType::OR},
    {"true",   TokenType::TRUE},
    {"false",  TokenType::FALSE},
    {"if",     TokenType::IF},
    {"else",   TokenType::ELSE},
    {"var",    TokenType::VAR},
    {"func",   TokenType::FUNC},
    {"class",  TokenType::CLASS},
    {"for",    TokenType::FOR},
    {"while",  TokenType::WHILE},
    {"nil",    TokenType::NIL},
    {"this",   TokenType::THIS},
    {"super",  TokenType::SUPER},
    {"return", TokenType::RETURN},
    {"print",  TokenType::PRINT},
  };

  const std::string source;
  std::vector<Token> tokens;

  int start = 0;
  int current = 0;
  int line = 1;

  // Character related
  [[nodiscard]] bool isAtEnd() const;
  [[nodiscard]] char peek() const;
  [[nodiscard]] char peekNext() const;
  char advance();
  bool match(char);

  // Token related
  void scanToken();
  void addToken(TokenType type);
  void addToken(TokenType type, Literal literal);
  void string();
  void number();
  void identifier();

  // Utility
  static bool isDigit(char);
  static bool isAlpha(char);
  static bool isAlphaNumeric(char);

public:
  explicit Scanner(std::string);

  std::vector<Token> scanTokens();
};

#endif //TRYND_SCANNER_H
