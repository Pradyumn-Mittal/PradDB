#include "page.h"

#include <cstring>
#include <iostream>

SlottedPage::SlottedPage()
{
  data.resize(PAGE_SIZE, 0);

  header.free_space_pointer = PAGE_SIZE;
  header.slot_count = 0;
}

bool SlottedPage::insert_row(
  const std::string& row
)
{
  uint16_t row_size =
    static_cast<uint16_t>(row.size());

  uint16_t slot_directory_end =
    sizeof(PageHeader) +
    (header.slot_count + 1) * sizeof(Slot);

  if (
    header.free_space_pointer <
    slot_directory_end + row_size
    )
  {
    return false;
  }

  header.free_space_pointer -= row_size;

  std::memcpy(
    data.data() + header.free_space_pointer,
    row.data(),
    row_size
  );

  Slot slot;

  slot.offset = header.free_space_pointer;
  std::cout
    << "INSERT OFFSET = "
    << slot.offset
    << "\n";
  slot.size = row_size;
  slot.is_deleted = 0;

  slots.push_back(slot);

  header.slot_count++;

  return true;
}

std::string SlottedPage::read_row(
  uint16_t slot_num
) const
{
  if (slot_num >= slots.size())
  {
    return "";
  }

  const Slot& slot = slots[slot_num];

  if (slot.is_deleted != 0)
  {
    return "";
  }

  std::cout
    << "READ OFFSET = "
    << slot.offset
    << "\n";

  std::string row;

  row.resize(slot.size);

  std::memcpy(
    row.data(),
    data.data() + slot.offset,
    slot.size
  );

  return row;
}

std::vector<char> SlottedPage::serialize() const
{
  std::vector<char> bytes = data;

  std::memcpy(
    bytes.data(),
    &header,
    sizeof(PageHeader)
  );

  if (!slots.empty())
  {
    std::memcpy(
      bytes.data() + sizeof(PageHeader),
      slots.data(),
      slots.size() * sizeof(Slot)
    );
  }

  return bytes;
}

void SlottedPage::deserialize(
  const std::vector<char>& bytes
)
{
  data = bytes;

  std::memcpy(
    &header,
    data.data(),
    sizeof(PageHeader)
  );

  slots.resize(header.slot_count);

  if (header.slot_count > 0)
  {
    std::memcpy(
      slots.data(),
      data.data() + sizeof(PageHeader),
      header.slot_count * sizeof(Slot)
    );
  }
}

uint16_t SlottedPage::slot_count() const
{
  return header.slot_count;
}