// The Algorithm Design Manual, Second Edition
// Programming Challenges
//
// 1-2. "The Trip" - Programming Challenges 110103, UVA Judge 10137.

#include <algorithm> // std::max
#include <cmath>     // std::ceil, std::floor
#include <iostream>
#include <numeric> // std::accumulate
#include <utility> // std::pair, std::make_pair
#include <vector>

using OneTripData = std::pair<int, std::vector<double>>;
using AllTripsData = std::vector<OneTripData>;
AllTripsData ReadInputs() {
  AllTripsData trips_expenses;
  for (;;) {
    int num;
    std::cin >> num;
    if (num == 0)
      break;

    std::vector<double> expenses;
    for (int i = 0; i < num; ++i) {
      double money;
      std::cin >> money;
      expenses.push_back(money);
    }

    trips_expenses.push_back(std::make_pair(num, expenses));
  }

  return trips_expenses;
}

std::vector<double> Calculate(const AllTripsData &trips_expenses) {
  std::vector<double> money_changes;
  for (auto &one_trip_expenses : trips_expenses) {
    auto [num, expenses] = one_trip_expenses; // C++17 参数化绑定
    double average = std::accumulate(expenses.cbegin(), expenses.cend(), 0.0) /
                     (num != 0 ? num : 1);
    // 分别累加低于和高于平均花销的值
    double greater_sum = 0.0, smaller_sum = 0.0;
    int greater_count = 0, smaller_count = 0;
    for (auto money : expenses) {
      double money_cent = money;
      if (money_cent < average) {
        smaller_sum += money_cent;
        ++smaller_count;
      } else if (average < money_cent) {
        greater_sum += money_cent;
        ++greater_count;
      }
    }

    double ceil_aver = std::ceil((average * 100)) / 100;
    double floor_aver = std::floor(average * 100) / 100;
    double greater_diff = greater_sum - ceil_aver * greater_count;
    double smaller_diff = floor_aver * smaller_count - smaller_sum;
    double change = std::max(greater_diff, smaller_diff);
    money_changes.push_back(change);
  }

  return money_changes;
}

void Outputs(const std::vector<double> &money_changes) {
  std::cout.precision(2);
  for (auto &change : money_changes) {
    std::cout << '$' << std::fixed << change << '\n';
  }
}

int main() {
  Outputs(Calculate(ReadInputs()));
  return 0;
}
