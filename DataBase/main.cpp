#include <iostream>
#include <iomanip>

#include "./imports/includes/api_exports.h"
#include "./imports/includes/query_result.h"

void print_separator(
  const std::vector<size_t>& widths
)
{
  std::cout << "+";

  for (size_t width : widths)
  {
    for (size_t i = 0;
      i < width + 2;
      i++)
    {
      std::cout << "-";
    }

    std::cout << "+";
  }

  std::cout << "\n";
}

void print_row(
  const std::vector<std::string>& row,
  const std::vector<size_t>& widths
)
{
  std::cout << "|";

  for (size_t i = 0;
    i < row.size();
    i++)
  {
    std::cout
      << " "
      << std::left
      << std::setw(
        static_cast<int>(widths[i])
      )
      << row[i]
      << " |";
  }

  std::cout << "\n";
}

void render_result(
  const QueryResult& result
)
{
  std::cout << "\n";

  std::cout
    << (
      result.success
      ? "SUCCESS"
      : "FAILED"
      )
    << "\n";

  std::cout
    << result.message
    << "\n";

  if (!result.columns.empty())
  {
    std::vector<size_t> widths(
      result.columns.size(),
      0
    );

    for (size_t i = 0;
      i < result.columns.size();
      i++)
    {
      widths[i] =
        result.columns[i].size();
    }

    for (const auto& row : result.rows)
    {
      for (size_t i = 0;
        i < row.size();
        i++)
      {
        widths[i] =
          std::max(
            widths[i],
            row[i].size()
          );
      }
    }

    std::cout << "\n";

    print_separator(widths);

    print_row(
      result.columns,
      widths
    );

    print_separator(widths);

    for (const auto& row : result.rows)
    {
      print_row(
        row,
        widths
      );
    }

    print_separator(widths);
  }

  std::cout
    << "\nExecution Time: "
    << result.execution_time_ms
    << " ms\n";
}

int main()
{
  if (!InitializeDB("database"))
  {
    std::cout
      << "DB Init Failed\n";

    return 1;
  }

  std::cout
    << "====================================\n";

  std::cout
    << "          PradDB CLI\n";

  std::cout
    << "====================================\n";

  std::cout
    << "Type 'exit' to quit\n\n";

  std::string query;

  while (true)
  {
    std::cout << "> ";

    std::getline(
      std::cin,
      query
    );

    if (query == "exit")
    {
      break;
    }

    QueryResult result =
      ExecuteQuery(
        query.c_str()
      );

    render_result(result);

    std::cout << "\n";
  }

  ShutdownDB();

  return 0;
}