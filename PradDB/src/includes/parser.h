#pragma once
#include <string>
#include <iostream>
#include "tokenizer.h"
#include "commands.h"

class SQLParser {
  std::vector<Token> _tokens;
  size_t pos = 0;

  // make tokens
  const std::vector<Token> makeToken(const std::string& input) { 
    std::vector<Token> s = tokenize(input);
    return s; 
  }

  // Helper funcs
  const Token& peek() { 
    if (pos >= _tokens.size()) {
      static Token eof = { TokenType::OPERATOR, "EOF" };
      return eof;
    }
    return _tokens[pos]; 
  }
  Token advance() { return _tokens[pos++]; }
  bool is_at_end() { return pos >= _tokens.size(); }
  void consume(TokenType type, std::string value = "") {
    if (!is_at_end() && peek().type == type && (peek().value == value || value == "")) {
      advance();
    }
    else {
      throw std::runtime_error("Unexpected token: " + peek().value);
    }
  }

  // command funcs
  CreateCmd parseCreate();
  InsertCmd parseInsert();
  SelectCmd parseSelect();

public:
  command parse(const std::string& input);
};