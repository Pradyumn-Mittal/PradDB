#include "../includes/api_exports.h"

#include "../includes/db.h"
#include "../includes/executor.h"

#include <memory>
#include <mutex>

static std::unique_ptr<DB> global_db = nullptr;

static std::unique_ptr<Executor> global_executor = nullptr;

static std::mutex api_mutex;

bool InitializeDB(const char* db_path)
{
  std::lock_guard<std::mutex> lock(api_mutex);

  if (global_db != nullptr)
  {
    return true;
  }

  global_db =
    std::make_unique<DB>();

  global_executor =
    std::make_unique<Executor>(
      *global_db
    );

  return true;
}

QueryResult ExecuteQuery(
  const char* query
)
{
  std::lock_guard<std::mutex> lock(api_mutex);

  QueryResult result;

  if (query == nullptr)
  {
    result.success = false;

    result.message =
      "Null query";

    return result;
  }

  result =
    global_executor->execute(
      query
    );
  return result;
}

void ShutdownDB()
{
  std::lock_guard<std::mutex> lock(api_mutex);

  global_executor.reset();

  global_db.reset();
}