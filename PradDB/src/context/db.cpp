#include "../includes/db.h"
#include <stdexcept>

bool DB::createTable(const std::string& name, const std::vector<Column>& columns) {
  if (tableExists(name)) return false;

  tables.emplace(name, Table{ name, columns, {} });
  return true;
}

bool DB::insert(const std::string& tableName, const std::vector<std::string>& rawValues) {
  auto it = tables.find(tableName);
  if (it == tables.end()) return false;

  Table& table = it->second;
  if (rawValues.size() != table.columns.size()) return false;

  Row newRow;
  newRow.values.reserve(rawValues.size());

  try {
    for (size_t i = 0; i < rawValues.size(); ++i) {
      if (table.columns[i].type == DataType::INT) {
        newRow.values.emplace_back(std::stoi(rawValues[i]));
      }
      else {
        newRow.values.emplace_back(rawValues[i]);
      }
    }
  }
  catch (const std::invalid_argument&) {
    return false;
  }
  catch (const std::out_of_range&) {
    return false;
  }

  table.rows.emplace_back(std::move(newRow));
  return true;
}

const Table* DB::getTable(const std::string& name) const {
  auto it = tables.find(name);
  if (it == tables.end()) return nullptr;
  return &it->second;
}

bool DB::tableExists(const std::string& name) const {
  return tables.find(name) != tables.end();
}

const std::unordered_map<std::string, Table>& DB::getTables() const {
  return tables;
} 