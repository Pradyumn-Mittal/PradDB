#include "table_file.h"

#include "page.h"
#include "serializer.h"
#include <iostream>
#include <array>

#include <fstream>

TableFile::TableFile(
  const std::string& path
)
{
  file_path = path;
}

bool TableFile::create()
{
  std::ofstream file(
    file_path,
    std::ios::binary
  );

  if (!file.is_open())
  {
    return false;
  }

  SlottedPage page;

  auto bytes = page.serialize();

  file.write(
    bytes.data(),
    PAGE_SIZE
  );

  file.close();

  return true;
}

bool TableFile::appendRow(
  const std::vector<std::string>& values
)
{
  std::fstream file(
    file_path,
    std::ios::binary |
    std::ios::in |
    std::ios::out
  );

  if (!file.is_open())
  {
    return false;
  }

  file.seekp(0, std::ios::end);

  std::streampos file_size =
    file.tellp();

  size_t total_pages =
    static_cast<size_t>(file_size) /
    PAGE_SIZE;

  if (total_pages == 0)
  {
    file.close();
    return false;
  }

  size_t last_page_index =
    total_pages - 1;

  file.seekg(
    last_page_index * PAGE_SIZE
  );

  std::array<char, PAGE_SIZE> buffer;

  file.read(
    buffer.data(),
    PAGE_SIZE
  );

  SlottedPage page;

  page.deserialize(
    std::vector<char>(
      buffer.begin(),
      buffer.end()
    )
  );

  std::string row =
    Serializer::serializeRow(values);

  if (!page.insert_row(row))
  {
    file.seekp(0, std::ios::end);

    SlottedPage new_page;

    new_page.insert_row(row);

    auto bytes =
      new_page.serialize();

    file.write(
      bytes.data(),
      PAGE_SIZE
    );

    file.close();

    return true;
  }

  auto bytes =
    page.serialize();

  file.seekp(
    last_page_index * PAGE_SIZE
  );

  file.write(
    bytes.data(),
    PAGE_SIZE
  );

  file.close();

  return true;
}

std::vector<std::vector<std::string>>
TableFile::readAllRows()
{
  std::vector<std::vector<std::string>>
    rows;

  std::ifstream file(
    file_path,
    std::ios::binary
  );

  if (!file.is_open())
  {
    return rows;
  }

  while (true)
  {
    std::vector<char> buffer(PAGE_SIZE);

    file.read(
      buffer.data(),
      PAGE_SIZE
    );

    if (file.gcount() != PAGE_SIZE)
    {
      break;
    }

    SlottedPage page;

    page.deserialize(buffer);

    for (
      uint16_t i = 0;
      i < page.slot_count();
      i++
      )
    {
      std::string raw =
        page.read_row(i);

      if (raw.empty())
      {
        continue;
      }

      rows.push_back(
        Serializer::deserializeRow(raw)
      );
    }
  }

  file.close();

  return rows;
}