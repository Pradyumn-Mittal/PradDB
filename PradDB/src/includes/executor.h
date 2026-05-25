#pragma once
#include <string>
#include <variant>
#include <iostream>
#include "parser.h"
#include "commands.h"
#include "db.h"
#include "display.h"

class Executor{
  DB& db;

public:

  Executor(DB& _db) : db(_db){}

  void execute(const std::string& input);
};

