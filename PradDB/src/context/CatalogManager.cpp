#include "../includes/CatalogManager.h"

#include <fstream>
#include <filesystem>
#include <unordered_set>

static const std::string CATALOG_FILE = "catalog.tbl";

bool CatalogManager::table_exists(
  const std::string& name
)
{
  std::ifstream file(CATALOG_FILE);

  if (!file.is_open())
  {
    return false;
  }

  std::string table;

  while (std::getline(file, table))
  {
    if (table == name)
    {
      file.close();
      return true;
    }
  }

  file.close();
  return false;
}

void CatalogManager::register_table(
  const std::string& name
)
{
  if (table_exists(name))
  {
    return;
  }

  std::ofstream file(
    CATALOG_FILE,
    std::ios::app
  );

  if (!file.is_open())
  {
    return;
  }

  file << name << "\n";

  file.close();
}

void CatalogManager::remove_table(
  const std::string& name
)
{
  std::ifstream file(CATALOG_FILE);

  if (!file.is_open())
  {
    return;
  }

  std::vector<std::string> tables;

  std::string table;

  while (std::getline(file, table))
  {
    if (table != name)
    {
      tables.push_back(table);
    }
  }

  file.close();

  std::ofstream out(
    CATALOG_FILE,
    std::ios::trunc
  );

  for (const auto& t : tables)
  {
    out << t << "\n";
  }

  out.close();
}

const std::vector<std::string>& CatalogManager::get_all_tables()
{
  static std::vector<std::string> result;

  result.clear();

  std::ifstream file(CATALOG_FILE);

  if (!file.is_open())
  {
    std::ofstream create(CATALOG_FILE);
    create.close();

    return result;
  }

  std::unordered_set<std::string> seen;

  std::string table;

  while (std::getline(file, table))
  {
    if (
      table.empty() ||
      seen.count(table)
      )
    {
      continue;
    }

    seen.insert(table);

    result.push_back(table);
  }

  file.close();

  return result;
}