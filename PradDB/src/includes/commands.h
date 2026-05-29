#pragma once
#include <variant>
#include <string>
#include <vector>

enum class CommandType {
  CREATE,
  INSERT,
  SELECT,
  SHOW_TABLES,
  DROP_TABLE,
  INVALID
};

struct Condition {
  std::string column;
  std::string op;      // "=", "<", ">", etc.
  std::string value;
};

struct CreateCmd {
  std::string table_name;
  std::vector<std::pair<std::string, std::string>> columns;
};

struct InsertCmd {
  std::string table_name;
  std::vector<std::string> values;
};

struct SelectCmd {
  std::string table_name;
  std::vector<std::string> column_names;
  std::vector<Condition> condition;
};

struct ShowTablesCmd {};

struct DropTableCmd {
  std::string table_name;
};

using command = std::variant<CreateCmd, InsertCmd, SelectCmd, ShowTablesCmd, DropTableCmd>;
