#pragma once

#include <string>
#include <vector>

struct QueryResult
{
  bool success = false;

  std::string message;

  std::vector<std::string> columns;

  std::vector<std::vector<std::string>> rows;

  double execution_time_ms = 0.0;
};