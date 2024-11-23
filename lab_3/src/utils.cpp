//
// Created by Aleksandr Mikhailov on 27.09.2024.
//

#include <fstream>

#include "utils.h"

std::vector<std::string> ReadFile(std::ifstream & in) {
    std::vector<std::string> result;
    std::string buffer;

    while (std::getline(in, buffer)) {
        result.push_back(std::move(buffer));
    }

    return result;
}

bool CheckEqualsOfStrings(const std::vector<std::string> & current, const std::vector<std::string> & expected) {
    if (current.size() != expected.size()) {
        return false;
    }

    for (int i = 0; i < current.size(); i++) {
        if (current[i] != expected[i]) {
            return false;
        }
    }

    return true;
}