#include "../includes/db.h"

#include "../storage/table_file.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace std;

bool DB::createTable(
  const std::string& table_name,
  const std::vector<Column>& columns
)
{
  if (tables.find(table_name) != tables.end())
  {
    return false;
  }

  std::string path = table_name + ".tbl";

  Table table;

  table.name = table_name;
  table.filePath = path;
  table.columns = columns;

  TableFile file(path);

  if (!file.create())
  {
    return false;
  }

  std::ofstream meta(
    table_name + ".meta"
  );

  if (!meta.is_open())
  {
    return false;
  }

  for (const auto& col : columns)
  {
    meta
      << col.name
      << "|"
      << static_cast<int>(col.type)
      << "\n";
    std::cout
      << "WRITING COLUMN = "
      << col.name
      << "\n";
  }
  meta.flush();
  

  meta.close();

  tables[table_name] = table;

  return true;
}

bool DB::insert(
  const std::string& table_name,
  const std::vector<std::string>& values
)
{
  Table* table = getTable(table_name);

  if (table == nullptr)
  {
    return false;
  }

  TableFile file(table->filePath);

  return file.appendRow(values);
}

Table* DB::getTable(
  const std::string& name
)
{
  auto it = tables.find(name);

  if (it != tables.end())
  {
    return &it->second;
  }

  std::string path =
    name + ".tbl";

  if (!std::filesystem::exists(path))
  {
    return nullptr;
  }

  Table table;

  table.name = name;
  table.filePath = path;

  std::ifstream meta(
    name + ".meta"
  );

  if (meta.is_open())
  {
    std::string line;

    while (std::getline(meta, line))
    {
      std::cout
        << "LOADED META LINE = "
        << line
        << "\n";

      size_t pos =
        line.find('|');

      if (pos == std::string::npos)
      {
        continue;
      }

      Column col;

      col.name =
        line.substr(0, pos);

      int type =
        std::stoi(
          line.substr(pos + 1)
        );

      col.type =
        static_cast<DataType>(type);

      table.columns.push_back(col);
      std::cout
        << "LOADED COLUMN = "
        << col.name
        << "\n";
    }

    meta.close();
  }

  tables[name] = table;

  return &tables[name];
}