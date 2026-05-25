#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <optional>

enum class DataType { INT, STRING };

using Cell = std::variant<int, std::string>;

struct Column {
  std::string name;
  DataType type;
};

struct Row {
  std::vector<Cell> values;
};

struct Table {
  std::string name;
  std::vector<Column> columns;
  std::vector<Row> rows;
};

class DB {
  std::unordered_map<std::string, Table> tables;

public:
  bool createTable(const std::string&, const std::vector<Column>&);
  bool insert(const std::string&, const std::vector<std::string>&);
  const Table* getTable(const std::string&) const;

  bool tableExists(const std::string&) const;
  const std::unordered_map<std::string, Table>& getTables() const;
};