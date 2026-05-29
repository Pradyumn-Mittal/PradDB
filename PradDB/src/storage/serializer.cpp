#include "serializer.h"

#include <cstdint>
#include <cstring>

static void write_uint32_le(
  std::string& buffer,
  uint32_t value
)
{
  for (int i = 0; i < 4; i++)
  {
    buffer.push_back(
      static_cast<char>(
        (value >> (i * 8)) & 0xFF
        )
    );
  }
}

static uint32_t read_uint32_le(
  const std::string& buffer,
  size_t& offset
)
{
  uint32_t value = 0;

  for (int i = 0; i < 4; i++)
  {
    value |=
      static_cast<uint32_t>(
        static_cast<unsigned char>(
          buffer[offset++]
          )
        ) << (i * 8);
  }

  return value;
}

std::string Serializer::serializeRow(
  const std::vector<std::string>& values
)
{
  std::string result;

  uint32_t column_count =
    static_cast<uint32_t>(
      values.size()
      );

  write_uint32_le(
    result,
    column_count
  );

  for (const auto& value : values)
  {
    uint32_t len =
      static_cast<uint32_t>(
        value.size()
        );

    write_uint32_le(
      result,
      len
    );

    result.append(value);
  }

  return result;
}

std::vector<std::string>
Serializer::deserializeRow(
  const std::string& row
)
{
  std::vector<std::string> values;

  size_t offset = 0;

  if (row.size() < 4)
  {
    return values;
  }

  uint32_t column_count =
    read_uint32_le(
      row,
      offset
    );

  for (uint32_t i = 0;
    i < column_count;
    i++)
  {
    if (offset + 4 > row.size())
    {
      break;
    }

    uint32_t len =
      read_uint32_le(
        row,
        offset
      );

    if (offset + len > row.size())
    {
      break;
    }

    values.push_back(
      row.substr(offset, len)
    );

    offset += len;
  }

  return values;
}