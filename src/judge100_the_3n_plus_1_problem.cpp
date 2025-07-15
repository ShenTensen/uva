// The Algorithm Design Manual, Second Edition
// Programming Challenges
//
// 1.1. "The 3n + 1 Problem" - Programming Challenges 110101, UVA Judge 100.
#include <iostream>
#include <vector>

int CountStep(int value) {
  int counter = 1;
  while (value != 1) {
    ++counter;
    if (value % 2 == 0) {
      value = value / 2;
    } else {
      value = value * 3 + 1;
    }
  }

  return counter;
}

int main() {
  std::vector<int> results;
  int first, last;
  const int kLeast = 1, kMost = 1000000;
  while (std::cin >> first >> last) {
    if (first < kLeast || last > kMost) {
      std::cerr << "first: " << first << " last: " << last << ", should among ["
                << kLeast << ", " << kMost << "]\n";
      continue;
    }

    int begin_index = first, end_index = last;
    if (last < first) {
      begin_index = last;
      end_index = first;
    }

    int biggest = 1;
    for (int i = begin_index; i < end_index + 1; ++i) {
      int step = CountStep(i);
      if (biggest < step) {
        biggest = step;
      }
    }

    results.push_back(first);
    results.push_back(last);
    results.push_back(biggest);
  }

  int count = 0;
  for (const auto &i : results) {
    std::cout << i << " ";
    if (++count % 3 == 0) {
      std::cout << '\n';
    }
  }

  return 0;
}
