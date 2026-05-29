#pragma once

#include <string>
#include <vector>

class Serializer
{
public:

  static std::string serializeRow(
    const std::vector<std::string>& values
  );

  static std::vector<std::string> deserializeRow(
    const std::string& row
  );
};