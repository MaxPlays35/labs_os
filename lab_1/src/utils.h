//
// Created by Aleksandr Mikhailov on 27.09.2024.
//
#pragma once

#ifndef UTILS_H
#define UTILS_H
#include <vector>

std::vector<std::string> ReadFile(std::ifstream & in);

bool CheckEqualsOfStrings(std::vector<std::string> & current, std::vector<std::string> & expected);

#endif //UTILS_H
