#pragma once

#include <string>

#include "db.h"
#include "query_result.h"

class Executor
{
private:

    DB& db;

public:

    bool debug_mode = true;

    Executor(DB& _db);

    QueryResult execute(const std::string& input);
};
