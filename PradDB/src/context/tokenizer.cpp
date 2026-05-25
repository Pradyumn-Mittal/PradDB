#include "../includes/tokenizer.h"

std::string toUpper(std::string s) {
  for (char& c : s) c = std::toupper(c);
  return s;
}

std::vector<Token> tokenize(const std::string& input) {
  std::vector<Token> tokens;

  std::regex token_regex(
    R"token("([^"]*)"|'([^']*)'|([<>!=]=)|([a-zA-Z_][a-zA-Z0-9_]*)|(\d+)|([,;().])|([=<>]))token"
  );

  auto words_begin = std::sregex_iterator(
    input.begin(), input.end(),
    token_regex
  );

  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    std::string str = match.str();

    if (match[1].matched || match[2].matched) {
      tokens.push_back({ TokenType::STRING, str });
    }
    else if (match[3].matched || match[7].matched) {
      tokens.push_back({ TokenType::OPERATOR, str });
    }
    else if (match[4].matched) {
      std::string upper = toUpper(str);
      if (keywords.count(upper)) {
        tokens.push_back({ TokenType::KEYWORD, upper });
      }
      else {
        tokens.push_back({ TokenType::IDENTIFIER, str });
      }
    }
    else if (match[5].matched) {
      tokens.push_back({ TokenType::NUMBER, str });
    }
    else if (match[6].matched) {
      char c = str[0];
      if (c == ',') tokens.push_back({ TokenType::COMMA, "," });
      else if (c == ';') tokens.push_back({ TokenType::SEMICOLON, ";" });
      else if (c == '(') tokens.push_back({ TokenType::LEFTPAREN, "(" });
      else if (c == ')') tokens.push_back({ TokenType::RIGHTPAREN, ")" });
      else if (c == '.') tokens.push_back({ TokenType::DOT, "." });
    }
  }

  return tokens;
}