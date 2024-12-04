//
// Created by Aleksandr Mikhailov on 04.12.2024.
//

#include <iostream>

#include "src/libraryFirst/libraryFirst.h"

int main() {
  int option = 0;
  int argument = 0;

  while (true) {
    std::cout << "Please enter an option: ";
    std::cin >> option;
    switch (option) {
      case -1:
        std::cout << "Thank you for using our program!\n";
        return 0;
      case 1:
        std::cin >> argument;
        std::cout << "The result of calculation: " << pi(argument) << std::endl;
        break;
      case 2:
        std::cin >> argument;
        std::cout << "The result of translation: " << translation(argument) << std::endl;
        break;
      default:
        std::cout << "Invalid option!" << std::endl;
        break;
    }
  }

  return 0;
}
