#pragma once
#include <iostream>
#include <string>
#include "db.h"
#include "executor.h"

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
      executor.execute(input);
    }
    catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n\n";
    }
  }
}