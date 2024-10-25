//
// Created by Aleksandr Mikhailov on 27.09.2024.
//
#pragma once
#include <vector>

std::vector<std::string> ReadFile(std::ifstream & in);

bool CheckEqualsOfStrings(const std::vector<std::string> & current, const std::vector<std::string> & expected);
