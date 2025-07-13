// The Algorithm Design Manual, Second Edition
// Programming Challenges
//
// 1-2. "The Trip" - Programming Challenges 110103, UVA Judge 10137.

#include <cmath> // std::round
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
    // 四舍五入平均花销到分
    // 分别累加低于和高于平均花销的差值
    double greater_sum = 0.0, less_sum = 0.0;
    int greater_count = 0, less_count = 0;
    for (auto money : expenses) {
      double money_cent = money;
      if (money_cent < average) {
        less_sum += money_cent;
        ++less_count;
      } else if (average < money_cent) {
        greater_sum += money_cent;
        ++greater_count;
      }
    }

    double round_aver = std::round(static_cast<double>((average * 100))) / 100;
    double greater_round_diff = greater_sum - round_aver * greater_count;
    double less_round_diff = round_aver * less_count - less_sum;
    double big = 0.0, little = 0.0;
    if (greater_round_diff < less_round_diff) {
      big = less_round_diff;
      little = greater_round_diff;
    } else {
      big = greater_round_diff;
      little = less_round_diff;
    }

    // 比较两个差值之间的差值的绝对值是否小于等于0.01
    // 如果是，结果就是四舍五入后的小的差值，反之为大的差值
    double change = 0.0;
    double greater_diff = greater_sum - average * greater_count;
    double less_diff = average * less_count - less_sum;
    if (-0.01 <= greater_diff - less_diff && greater_diff - less_diff <= 0.01) {
      change = little;
    } else {
      change = big;
    }

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
