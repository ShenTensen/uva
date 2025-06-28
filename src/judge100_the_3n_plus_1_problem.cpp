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
  while (std::cin >> first >> last) {
    if (first < 1 || last > 10000 || last < first) {
      std::cerr << "first: " << first << " last: " << last
                << ", should among [0, 10000]!" << std::endl;
      break;
    }

    int biggest = 1;
    for (int i = first; i < last + 1; ++i) {
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
      std::cout << std::endl;
    }
  }

  return 0;
}
