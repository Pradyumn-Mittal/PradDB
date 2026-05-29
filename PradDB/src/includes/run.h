#pragma once
#include <iostream>
#include <string>
#include "db.h"
#include "executor.h"
#include <chrono>

void runDB() {
  DB myDatabase;
  Executor executor(myDatabase);
  while (true) {
    std::string input;
    std::cout << "> > > ";
    std::getline(std::cin, input);

    if (toUpper(input) == "EXIT") {
      break;
    }
    try{
      auto start =
        std::chrono::high_resolution_clock::now();

      executor.execute(input);

      auto end =
        std::chrono::high_resolution_clock::now();

      auto duration =
        std::chrono::duration_cast<
        std::chrono::microseconds
        >(end - start);

      std::cout
        << "\nExecution Time: "
        << float(duration.count())/ 1000000
        << "s\n";
    }
    catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n\n";
    }
  }
}