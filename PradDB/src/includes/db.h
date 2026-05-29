
#pragma once

#include "CatalogManager.h"
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
  CatalogManager Cm;

public:
  bool createTable(
    const std::string& name,
    const std::vector<Column>& columns);

  bool insert(
    const std::string& tableName,
    const std::vector<std::string>& rawValues);

  Table* getTable(
    const std::string& name);

  const std::vector<std::string>& show_tables() {
    return Cm.get_all_tables();
  }

  bool dropTable(
    const std::string& name);

};
