#pragma once
#include <vector>
#include <string>
#include <cctype>
#include <unordered_set>
#include <regex>

enum class TokenType {
  KEYWORD,
  IDENTIFIER,
  NUMBER,
  STRING,
  OPERATOR,
  COMMA,
  SEMICOLON,
  LEFTPAREN,
  RIGHTPAREN,
  DOT
};

struct Token {
  TokenType type;
  std::string value;
};

// Only required keywords
static const std::unordered_set<std::string> keywords = {
    "CREATE", "TABLE",
    "INSERT", "INTO", "VALUES",
    "SELECT", "FROM", "WHERE",
    "SHOW", "TABLES",
    "DROP", "TABLE"
};

std::string toUpper(std::string s);

std::vector<Token> tokenize(const std::string& input);
