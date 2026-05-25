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

    executor.execute(input);
  }
}