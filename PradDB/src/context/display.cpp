#include "../includes/display.h"

#include "../storage/table_file.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

static bool checkCondition(
  const std::vector<std::string>& row,
  const Table& table,
  const std::vector<Condition>& conditions
)
{
  if (conditions.empty())
  {
    return true;
  }

  for (const auto& cond : conditions)
  {
    int column_index = -1;

    for (size_t i = 0; i < table.columns.size(); i++)
    {
      if (table.columns[i].name == cond.column)
      {
        column_index =
          static_cast<int>(i);

        break;
      }
    }

    if (column_index == -1)
    {
      return false;
    }

    const std::string& row_value =
      row[column_index];

    const std::string& condition_value =
      cond.value;

    if (cond.op == "=")
    {
      if (row_value != condition_value)
      {
        return false;
      }
    }
    else if (cond.op == "!=")
    {
      if (row_value == condition_value)
      {
        return false;
      }
    }
    else if (cond.op == ">")
    {
      if (!(std::stoi(row_value) >
        std::stoi(condition_value)))
      {
        return false;
      }
    }
    else if (cond.op == "<")
    {
      if (!(std::stoi(row_value) <
        std::stoi(condition_value)))
      {
        return false;
      }
    }
    else if (cond.op == ">=")
    {
      if (!(std::stoi(row_value) >=
        std::stoi(condition_value)))
      {
        return false;
      }
    }
    else if (cond.op == "<=")
    {
      if (!(std::stoi(row_value) <=
        std::stoi(condition_value)))
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}


void printTable(
  DB& db,
  const std::string& table_name,
  const std::vector<std::string>& column_names,
  const std::vector<Condition>& conditions
)
{
  std::cout << "NEW DISPLAY CPP LOADED\n";
  Table* table =
    db.getTable(table_name);

  if (table == nullptr)
  {
    std::cout << "Table not found.\n";
    return;
  }

  TableFile file(
    table->filePath
  );

  auto rows =
    file.readAllRows();

  if (rows.empty())
  {
    std::cout << "Empty table.\n";
    return;
  }

  std::cout
    << "COLUMN COUNT = "
    << table->columns.size()
    << "\n";

  for (const auto& col : table->columns)
  {
    std::cout
      << "COLUMN = ["
      << col.name
      << "]\n";
  }

  std::vector<int> selected_indices;

  bool select_all = false;

  if (column_names.empty())
  {
    select_all = true;
  }
  else
  {
    bool all_empty = true;

    for (const auto& raw : column_names)
    {
      std::string cleaned;

      for (char ch : raw)
      {
        if (
          !std::isspace(
            static_cast<unsigned char>(ch)
          )
          &&
          ch != ';'
          )
        {
          cleaned += ch;
        }
      }

      std::cout
        << "RAW TOKEN = ["
        << raw
        << "]\n";

      std::cout
        << "CLEANED TOKEN = ["
        << cleaned
        << "]\n";

      if (!cleaned.empty())
      {
        all_empty = false;
      }

      if (cleaned == "*")
      {
        select_all = true;
      }
    }

    if (all_empty)
    {
      select_all = true;
    }
  }

  if (select_all)
  {
    std::cout
      << "SELECT ALL ACTIVE\n";

    for (
      size_t i = 0;
      i < table->columns.size();
      i++
      )
    {
      selected_indices.push_back(
        static_cast<int>(i)
      );

      std::cout
        << "SELECTED INDEX = "
        << i
        << "\n";
    }
  }
  else
  {
    for (const auto& raw_name : column_names)
    {
      std::string cleaned;

      for (char ch : raw_name)
      {
        if (
          !std::isspace(
            static_cast<unsigned char>(ch)
          )
          &&
          ch != ';'
          )
        {
          cleaned += ch;
        }
      }

      for (
        size_t i = 0;
        i < table->columns.size();
        i++
        )
      {
        if (
          table->columns[i].name
          ==
          cleaned
          )
        {
          selected_indices.push_back(
            static_cast<int>(i)
          );

          std::cout
            << "MATCHED COLUMN = "
            << cleaned
            << "\n";
        }
      }
    }
  }

  std::cout
    << "FINAL SELECTED COUNT = "
    << selected_indices.size()
    << "\n";
  

  std::vector<size_t> widths(
    selected_indices.size(),
    5
  );

  for (
    size_t i = 0;
    i < selected_indices.size();
    i++
    )
  {
    widths[i] =
      table->columns[
        selected_indices[i]
      ].name.size();
  }

  for (const auto& row : rows)
  {
    if (!checkCondition(
      row,
      *table,
      conditions
    ))
    {
      continue;
    }

    for (
      size_t i = 0;
      i < selected_indices.size();
      i++
      )
    {
      int index =
        selected_indices[i];

      widths[i] =
        std::max(
          widths[i],
          row[index].size()
        );
    }
  }

  std::cout << "\n";

  for (size_t i = 0; i < widths.size(); i++)
  {
    std::cout
      << "+"
      << std::string(widths[i] + 2, '-');
  }

  std::cout << "+\n";

  for (
    size_t i = 0;
    i < selected_indices.size();
    i++
    )
  {
    std::cout
      << "| "
      << std::left
      << std::setw((int)widths[i])
      << table->columns[
        selected_indices[i]
      ].name
      << " ";
  }

  std::cout << "|\n";

  for (size_t i = 0; i < widths.size(); i++)
  {
    std::cout
      << "+"
      << std::string(widths[i] + 2, '-');
  }

  std::cout << "+\n";

  for (const auto& row : rows)
  {
    if (!checkCondition(
      row,
      *table,
      conditions
    ))
    {
      continue;
    }

    for (
      size_t i = 0;
      i < selected_indices.size();
      i++
      )
    {
      int index =
        selected_indices[i];

      std::cout
        << "| "
        << std::left
        << std::setw((int)widths[i])
        << row[index]
        << " ";
    }

    std::cout << "|\n";
  }

  for (size_t i = 0; i < widths.size(); i++)
  {
    std::cout
      << "+"
      << std::string(widths[i] + 2, '-');
  }

  std::cout << "+\n\n";
}

void showTables(
  DB& db
)
{
  auto& tables = db.show_tables();
  if (tables.empty())
  {
    std::cout << "No tables found.\n";
    return;
  }
  std::cout << "Tables:\n";
  for (const auto& name : tables)
  {
    std::cout << "- " << name << "\n";
  }
  std::cout << "\n";
}