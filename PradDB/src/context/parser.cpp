#include "../includes/parser.h"

command SQLParser::parse(const std::string& input) {
  pos = 0;
  _tokens = makeToken(input);

  //std::cout << "Lexer found " << _tokens.size() << " tokens." << std::endl;
  //for (const auto& t : _tokens) std::cout << "[" << t.value << "] ";
  //std::cout << std::endl;

  if (is_at_end()) {
    throw std::runtime_error("Empty query");
  }

  const std::string& cmd = toUpper(peek().value);
  if (cmd == "CREATE") return parseCreate();
  if (cmd == "INSERT") return parseInsert();
  if (cmd == "SELECT") return parseSelect();
  if (cmd == "SHOW") return parseShowTables();
  if (cmd == "DROP") return parseDropTable();
  throw std::runtime_error("Unknown command: " + cmd);
}

CreateCmd SQLParser::parseCreate() {
  CreateCmd cmd;
  consume(TokenType::KEYWORD, "CREATE");
  consume(TokenType::KEYWORD, "TABLE");

  cmd.table_name = advance().value;

  consume(TokenType::LEFTPAREN);
  while (peek().type != TokenType::RIGHTPAREN) {
    std::string colName = advance().value;
    std::string colType = advance().value;
    cmd.columns.push_back({colName, colType});

    if (peek().type == TokenType::COMMA) consume(TokenType::COMMA);
    if (is_at_end()) throw std::runtime_error("Missing closing parenthesis!");
  }
  consume(TokenType::RIGHTPAREN);
  //consume(TokenType::SEMICOLON);

  return cmd;
}

InsertCmd SQLParser::parseInsert() {
  InsertCmd cmd;
  consume(TokenType::KEYWORD, "INSERT");
  consume(TokenType::KEYWORD, "INTO");

  cmd.table_name = advance().value;

  consume(TokenType::KEYWORD, "VALUES");

  consume(TokenType::LEFTPAREN);
  while (peek().type != TokenType::RIGHTPAREN) {
    std::string value = advance().value;
    cmd.values.push_back(value);

    if (peek().type == TokenType::COMMA) consume(TokenType::COMMA);
    if (is_at_end()) throw std::runtime_error("Missing closing parenthesis!");
  }
  consume(TokenType::RIGHTPAREN);
  //consume(TokenType::SEMICOLON);

  return cmd;
}

SelectCmd SQLParser::parseSelect() {
  SelectCmd cmd;

  consume(TokenType::KEYWORD, "SELECT");

  // Parse column list
  if (peek().value == "*") {

    cmd.column_names.push_back("*");

    advance();
  }
  else {
    while (true) {
      if (peek().value == "FROM") break;

      Token col = peek();
      consume(TokenType::IDENTIFIER);
      cmd.column_names.push_back(col.value);

      if (peek().value == ",") {
        advance(); // skip comma
      }
      else {
        break;
      }
    }
  }

  consume(TokenType::KEYWORD, "FROM");

  // Table name
  Token table = peek();
  consume(TokenType::IDENTIFIER);
  cmd.table_name = table.value;

  // Optional WHERE clause
  if (!is_at_end() && peek().value == "WHERE") {
    advance(); // consume WHERE

    while (true) {
      Condition cond;

      Token col = peek();
      consume(TokenType::IDENTIFIER);
      cond.column = col.value;

      Token op = advance(); // should validate operator type
      cond.op = op.value;

      Token val = advance(); // should validate literal/identifier
      std::cout << "Condition value token: [" << val.value << "] of type " << static_cast<int>(val.type) << std::endl;
      if (val.value.empty() or val.type == TokenType::SEMICOLON) throw std::runtime_error("Invalid Syntax");
      cond.value = val.value;

      cmd.condition.push_back(cond);

      // Handle multiple conditions (AND only for now)
      if (peek().value == "AND") {
        advance();
      }
      else {
        break;
      }
    }
  }

  if (!is_at_end() && peek().value == ";") {
    advance();
  }

  return cmd;
}

ShowTablesCmd SQLParser::parseShowTables() {
  ShowTablesCmd cmd;
  consume(TokenType::KEYWORD, "SHOW");
  consume(TokenType::KEYWORD, "TABLES");
  //consume(TokenType::SEMICOLON);
  return cmd;
}

DropTableCmd SQLParser::parseDropTable() {
  DropTableCmd cmd;
  consume(TokenType::KEYWORD, "DROP");
  consume(TokenType::KEYWORD, "TABLE");
  cmd.table_name = advance().value;
  //consume(TokenType::SEMICOLON);
  return cmd;
}