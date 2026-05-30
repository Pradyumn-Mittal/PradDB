#include "../includes/executor.h"

#include "../includes/parser.h"

#include <chrono>
#include <sstream>
#include <variant>

Executor::Executor(DB& _db)
  : db(_db)
{}

QueryResult Executor::execute(const std::string& input)
{
  QueryResult result;

  auto start =
    std::chrono::high_resolution_clock::now();

  try
  {
    SQLParser parser;

    command cmd = parser.parse(input);

    std::visit(
      [&](auto&& command_obj)
      {
        using T =
          std::decay_t<decltype(command_obj)>;

        if constexpr (
          std::is_same_v<T, CreateCmd>
          )
        {
          std::vector<Column> columns;

          for (auto const& col : command_obj.columns)
          {
            Column c;

            c.name = col.first;

            if (col.second == "INT")
            {
              c.type = DataType::INT;
            }
            else
            {
              c.type = DataType::STRING;
            }

            columns.push_back(c);
          }

          bool created =
            db.createTable(
              command_obj.table_name,
              columns
            );

          result.success = created;

          result.message =
            created
            ? "Table created successfully"
            : "Table creation failed";
        }

        else if constexpr (
          std::is_same_v<T, InsertCmd>
          )
        {
          bool inserted =
            db.insert(
              command_obj.table_name,
              command_obj.values
            );

          result.success = inserted;

          result.message =
            inserted
            ? "Insert successful"
            : "Insert failed";
        }

        else if constexpr (
          std::is_same_v<T, SelectCmd>
          )
        {
          Table* table =
            db.getTable(
              command_obj.table_name
            );

          if (table == nullptr)
          {
            result.success = false;

            result.message =
              "Table not found";

            return;
          }

          for (const auto& col : table->columns)
          {
            result.columns.push_back(
              col.name
            );
          }

          auto rows =
            db.select(
              command_obj.table_name
            );

          result.success = true;

          for (const auto& row : rows)
          {
            std::vector<std::string> row_data;

            for (const auto& cell : row.values)
            {
              if (std::holds_alternative<int>(cell))
              {
                row_data.push_back(
                  std::to_string(
                    std::get<int>(cell)
                  )
                );
              }
              else
              {
                row_data.push_back(
                  std::get<std::string>(cell)
                );
              }
            }

            result.rows.push_back(
              row_data
            );
          }

          result.message =
            "Select successful";
        }

        else if constexpr (
          std::is_same_v<T, ShowTablesCmd>
          )
        {
          const auto& tables =
            db.show_tables();

          result.success = true;

          result.columns.push_back(
            "Tables"
          );

          for (const auto& table : tables)
          {
            result.rows.push_back(
              { table }
            );
          }

          result.message =
            "Tables fetched";
        }

        else if constexpr (
          std::is_same_v<T, DropTableCmd>
          )
        {
          bool dropped =
            db.dropTable(
              command_obj.table_name
            );

          result.success = dropped;

          result.message =
            dropped
            ? "Table dropped successfully"
            : "Table drop failed";
        }

        else
        {
          result.success = false;

          result.message =
            "Unknown command type";
        }
      },
      cmd
    );

    auto end =
      std::chrono::high_resolution_clock::now();

    auto duration =
      std::chrono::duration_cast
      <
      std::chrono::microseconds
      >(end - start);

    auto execution_time =
      duration.count() / 1000.0;

    result.execution_time_ms =
      execution_time;
  }
  catch (const std::exception& e)
  {
    result.success = false;

    result.message =
      std::string("Exception: ") + e.what();
  }

  return result;
}