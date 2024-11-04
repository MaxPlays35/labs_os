//
// Created by Aleksandr Mikhailov on 26.10.2024.
//
#pragma once
#include <vector>

struct ThreadWork {
  std::shared_ptr<std::vector<int>> array;
  int left;
  int right;
};

void* threadRoutine(void *arg);
