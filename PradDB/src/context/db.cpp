#include "../includes/db.h"

#include "../storage/table_file.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <system_error>

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
    //std::cout
    //  << "WRITING COLUMN = "
    //  << col.name
    //  << "\n";
  }

  meta.close();

  tables[table_name] = table;

  Cm.register_table(table_name);

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
      //std::cout
      //  << "LOADED META LINE = "
      //  << line
      //  << "\n";

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
    //  std::cout
    //    << "LOADED COLUMN = "
    //    << col.name
    //    << "\n";
    }

    meta.close();
  }

  tables[name] = table;

  if (!Cm.table_exists(name))
  {
    Cm.register_table(name);
  }

  return &tables[name];
}

bool DB::dropTable(
  const std::string& name
)
{
  auto it = tables.find(name);

  if (it == tables.end())
  {
    std::string path =
      name + ".tbl";

    if (!std::filesystem::exists(path))
    {
      return false;
    }
  }

  // REMOVE FROM MEMORY FIRST
  tables.erase(name);

  // FORCE TEMPORARY FILE HANDLE RELEASE
  std::this_thread::sleep_for(
    std::chrono::milliseconds(1)
  );

  std::error_code ec1;
  std::error_code ec2;

  std::filesystem::remove(
    name + ".tbl",
    ec1
  );

  std::filesystem::remove(
    name + ".meta",
    ec2
  );

  Cm.remove_table(name);

  return !ec1;
}

std::vector<Row> DB::select(
  const std::string& tableName)
{
  std::vector<Row> rows;

  Table* table =
    getTable(tableName);

  if (table == nullptr)
  {
    return rows;
  }

  TableFile tf(
    table->filePath
  );

  auto rawRows =
    tf.readAllRows();

  for (const auto& rawRow : rawRows)
  {
    Row row;

    for (size_t i = 0;
      i < rawRow.size();
      i++)
    {
      const auto& column =
        table->columns[i];

      if (column.type == DataType::INT)
      {
        row.values.push_back(
          std::stoi(rawRow[i])
        );
      }
      else
      {
        row.values.push_back(
          rawRow[i]
        );
      }
    }

    rows.push_back(row);
  }

  return rows;
}