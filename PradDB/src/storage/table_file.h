#pragma once

#include <string>
#include <vector>

class TableFile
{
public:

  TableFile(
    const std::string& path
  );

  bool create();

  bool appendRow(
    const std::vector<std::string>& values
  );

  std::vector<std::vector<std::string>>
    readAllRows();

private:

  std::string file_path;
};