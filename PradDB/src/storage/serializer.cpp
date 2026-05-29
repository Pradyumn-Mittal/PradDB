#include "serializer.h"

#include <sstream>

std::string Serializer::serializeRow(
  const std::vector<std::string>& values
)
{
  std::string result;

  for (size_t i = 0; i < values.size(); i++)
  {
    result += values[i];

    if (i != values.size() - 1)
    {
      result += "|";
    }
  }

  return result;
}

std::vector<std::string> Serializer::deserializeRow(
  const std::string& row
)
{
  std::vector<std::string> values;

  std::stringstream ss(row);

  std::string item;

  while (std::getline(ss, item, '|'))
  {
    values.push_back(item);
  }

  return values;
}