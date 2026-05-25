#include "../includes/display.h"
#include <iostream>
#include <unordered_map>
#include <sstream>


// Assumes Cell = std::variant<int, double, std::string> (or similar)

bool evaluateCondition(const Cell& cell, const Condition& cond) {
  return std::visit([&](auto&& value) -> bool {
    using T = std::decay_t<decltype(value)>;

    std::stringstream ss(cond.value);
    T rhs{};

    ss >> rhs;

    if (cond.op == "=") return value == rhs;
    if (cond.op == ">") return value > rhs;
    if (cond.op == "<") return value < rhs;

    return false;
    }, cell);
}

void printTable(DB& db,
  const std::string& table_name,
  const std::vector<std::string>& column_names,
  const std::vector<Condition>& conditions)
{
  const Table* table = db.getTable(table_name);

  if (!table) {
    std::cerr << "Error: Table '" << table_name << "' not found.\n";
    return;
  }

  const auto& cols = table->columns;
  const auto& rows = table->rows;

  // ---------------------------
  // Build column name → index map (O(n))
  // ---------------------------
  std::unordered_map<std::string, int> col_index_map;
  for (size_t i = 0; i < cols.size(); ++i) {
    col_index_map[cols[i].name] = static_cast<int>(i);
  }

  // ---------------------------
  // Resolve selected columns
  // ---------------------------
  std::vector<int> selected_indices;

  if (column_names.empty()) {
    selected_indices.reserve(cols.size());
    for (int i = 0; i < cols.size(); i++)
      selected_indices.push_back(i);
  }
  else {
    selected_indices.reserve(column_names.size());

    for (const auto& name : column_names) {
      auto it = col_index_map.find(name);
      if (it == col_index_map.end()) {
        std::cerr << "Error: Column '" << name << "' not found.\n";
        return;
      }
      selected_indices.push_back(it->second);
    }
  }

  // ---------------------------
  // Pre-resolve condition column indices
  // ---------------------------
  struct CompiledCondition {
    int col_index;
    Condition cond;
  };

  std::vector<CompiledCondition> compiled_conditions;
  compiled_conditions.reserve(conditions.size());

  for (const auto& cond : conditions) {
    auto it = col_index_map.find(cond.column);
    if (it == col_index_map.end()) {
      std::cerr << "Error: Column '" << cond.column << "' not found in WHERE.\n";
      return;
    }

    compiled_conditions.push_back({ it->second, cond });
  }

  // ---------------------------
  // Header
  // ---------------------------
  for (int idx : selected_indices) {
    std::cout << cols[idx].name << "\t";
  }
  std::cout << "\n";

  // ---------------------------
  // Rows
  // ---------------------------
  for (const auto& row : rows) {

    // WHERE filtering (AND logic)
    bool match = true;
    for (const auto& cc : compiled_conditions) {
      if (!evaluateCondition(row.values[cc.col_index], cc.cond)) {
        match = false;
        break;
      }
    }

    if (!match) continue;

    // Print selected columns only
    for (int idx : selected_indices) {
      std::visit([](auto&& arg) {
        std::cout << arg << "\t";
        }, row.values[idx]);
    }

    std::cout << "\n\n";
  }
}