//
// Created by Aleksandr Mikhailov on 04.12.2024.
//
#include <iostream>
#include <istream>
#include <ostream>

#include "../libraryFirst/libraryFirst.h"

void staticWork(std::istream & in, std::ostream & out) {
    int option = 0;
    int argument = 0;

    while (true) {
        out << "Please enter an option: ";
        in >> option;
        switch (option) {
            case -1:
                out << "Thank you for using program!\n";
                return;
            case 1:
                in >> argument;
                out << "The result of calculation: " << pi(argument) << std::endl;
            break;
            case 2:
                in >> argument;
                out << "The result of translation: " << translation(argument) << std::endl;
            break;
            default:
                out << "Invalid option!" << std::endl;
            break;
        }
    }
}

int main() {
    staticWork(std::cin, std::cout);

    return 0;
}