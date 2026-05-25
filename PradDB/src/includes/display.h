#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "db.h"
#include "commands.h"

void printTable(DB& db, const std::string& table_name , const std::vector<std::string>& column_names, const std::vector<Condition>& condition);

bool evaluateCondition(const Cell& cell, const Condition& cond);