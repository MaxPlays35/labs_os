//
// Created by MaxPlays on 27/09/2024.
//

#include <iostream>
#include <ostream>
#include <random>
#include <vector>

#include "mergesortthreaded/mergesortthreaded.h"
#include "src/mergesort/mergesort.h"
#include "src/threadwork/threadwork.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " max quantity of threads" << std::endl;
    return 1;
  }

  int threads = std::atoi(argv[1]);

  if (threads <= 0) {
    std::cerr << "Invalid number of threads" << std::endl;
    return 1;
  }

  std::cout << "Using " << threads << " threads" << std::endl;

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(1, 20);

  std::vector<ThreadWork> work;
  std::vector<pthread_t> threads_array(threads);

  size_t size = 1e7;
  std::vector<int> v(size);
  for (auto i = 0; i < size; ++i){
    v[i] = dist(mt);
  }

  const auto pointer = std::make_shared<std::vector<int>>(v);

  const auto time = mergeSortThreaded(pointer, threads);

  std::cout << "Elapsed time: " << time << "ms";
  std::cout << std::endl;




  return 0;
}
