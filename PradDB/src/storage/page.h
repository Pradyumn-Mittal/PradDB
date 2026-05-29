#pragma once

#include <vector>
#include <string>
#include <cstdint>

const uint32_t PAGE_SIZE = 4096;

#pragma pack(push, 1)

struct Slot
{
  uint16_t offset;
  uint16_t size;
  uint8_t is_deleted;
};

#pragma pack(pop)

struct PageHeader
{
  uint16_t free_space_pointer;
  uint16_t slot_count;
};

class SlottedPage
{
public:

  SlottedPage();

  bool insert_row(
    const std::string& row
  );

  std::string read_row(
    uint16_t slot_num
  ) const;

  std::vector<char> serialize() const;

  void deserialize(
    const std::vector<char>& bytes
  );

  uint16_t slot_count() const;

private:

  PageHeader header;

  std::vector<Slot> slots;

  std::vector<char> data;
};