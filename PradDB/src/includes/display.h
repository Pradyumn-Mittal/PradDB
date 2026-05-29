#pragma once

#include "../includes/db.h"
#include "../includes/commands.h"


void printTable(
  DB& db,
  const std::string& table_name,
  const std::vector<std::string>& column_names,
  const std::vector<Condition>& conditions
);