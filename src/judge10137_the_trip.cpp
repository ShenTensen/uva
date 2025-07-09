// The Algorithm Design Manual, Second Edition
// Programming Challenges
//
// 1-2. "The Trip" - Programming Challenges 110103, UVA Judge 10137.

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

    double changes = 0.0;
    double average = std::accumulate(expenses.cbegin(), expenses.cend(), 0.0) /
                     (num != 0 ? num : 1);
    for (auto expence : expenses) {
      if (expence < average) {
        changes += (average - expence);
      }
    }

    money_changes.push_back(changes);
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
