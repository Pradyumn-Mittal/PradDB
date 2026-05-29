#include "../includes/executor.h"

void Executor::execute(const std::string& input) {
  SQLParser parser;
  command cmd = parser.parse(input);

  try {
    std::visit([this](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, CreateCmd>) {
        std::vector<Column> db_cols;
        for (const auto& [name, typeStr] : arg.columns) {
          DataType type = (toUpper(typeStr) == "INT") ? DataType::INT : DataType::STRING;
          db_cols.push_back({ name, type });
        }

        if (db.createTable(arg.table_name, db_cols)) {
          std::cout << "Creation of table " << arg.table_name << " was successfull\n\n";
        }
        else {
          std::cout << "Creation of table " << arg.table_name << " was not successfull\n\n";
        }
      }
      else if constexpr (std::is_same_v<T, InsertCmd>) {
        if (db.insert(arg.table_name, arg.values)) {
          std::cout << "Insertion in to table " << arg.table_name << " was successfull\n\n";
        }
        else {
          std::cout << "Insertion in to table " << arg.table_name << " was not successfull\n\n";
        }
      }
      else if constexpr (std::is_same_v<T, SelectCmd>) {
        const Table* table = db.getTable(arg.table_name);

        if (table == nullptr) {
          std::cout << "Error: Table '" << arg.table_name << "' not found.\n";
          return;
        }

        printTable(db, arg.table_name, arg.column_names, arg.condition);
      }
      }, cmd);
  }
  catch (const std::runtime_error& e) {
    // This catches the "Unexpected token" error from your consume function
    std::cerr << "SQL Error: \n" << e.what() << std::endl;
  }
  catch (...) {
    std::cerr << "An unknown error occurred during execution.\n" << std::endl;
  }
}