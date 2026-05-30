#pragma once

#ifdef PRADDB_EXPORTS
#define PRADDB_API __declspec(dllexport)
#else
#define PRADDB_API __declspec(dllimport)
#endif

#include "query_result.h"


PRADDB_API bool InitializeDB(
  const char* db_path
);

PRADDB_API void ShutdownDB();

PRADDB_API QueryResult ExecuteQuery(
  const char* query
);