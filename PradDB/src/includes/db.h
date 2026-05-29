
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

enum class DataType {
  INT = 0,
  STRING = 1
};

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

  std::string filePath;

  uint32_t pageCount = 0;
};

class DB {
  std::unordered_map<std::string, Table> tables;

public:

  void loadAllTables();

  bool createTable(
    const std::string& name,
    const std::vector<Column>& columns);

  bool insert(
    const std::string& tableName,
    const std::vector<std::string>& rawValues);

  std::vector<Row> scanTable(
    const std::string& tableName);

  Table* getTable(
    const std::string& name);

  bool tableExists(
    const std::string& name) const;
};
