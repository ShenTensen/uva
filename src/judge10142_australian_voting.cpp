// The Algorithm Design Manual, Second Edition
// Progamming Challenges
//
// 1-3 "Australian Voting" - Programming Challenges 110108, UVA Judge 10142.

#include <cassert>  // assert
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>  // std::pair, std::make_pair
#include <vector>

using ResultOneCase = std::vector<std::string>;
using Results = std::vector<ResultOneCase>;
using Candidates = std::vector<std::string>;
using Ballots = std::vector<std::list<int>>;
using OneCase = std::pair<Candidates, Ballots>;
using Cases = std::vector<OneCase>;

Cases ReadInput();
Results Process(const Cases& cases);
void OutputResults(const Results& results);
void OutputCases(const Cases& cases);  // 调试使用。

int main() {
  Cases voting_cases = ReadInput();  
  Results results = Process(voting_cases);
  OutputResults(results);
  return 0;
}

Cases ReadInput() {
  int case_count;
  std::cin >> case_count;
  Cases voting_cases;
  for (int i = 0; i < case_count; ++i) {
    int candidates_count;
    std::cin >> candidates_count;  // 读取被选举人人数之后，留下了一个换行符。
    std::string name;
    std::getline(std::cin, name);
    assert(name.empty());
    Candidates candidates;
    for (int i = 0; i < candidates_count; ++i) {
      std::getline(std::cin, name);
      candidates.push_back(name);
    }

    std::string line;
    Ballots ballots;
    while (std::getline(std::cin, line) && !line.empty()) {
      int order;
      std::list<int> vote;
      std::istringstream ballot(line);
      while (ballot >> order) {
        vote.push_back(order);
      }

      ballots.push_back(vote);
    }

    voting_cases.push_back(std::make_pair(candidates, ballots));
  }

  std::cout << std::endl;
  return voting_cases;
}


void OutputCases(const Cases& voting_cases) {
  // 打印读取的内容，方便测试。
  std::cout << "输入的数据：" << '\n';
  for (auto one_case : voting_cases) {
    for (auto name: one_case.first) {
      std::cout << name << '\n';
    }

    for (auto ballot : one_case.second) {
      for (auto index : ballot) {
        std::cout << index << ' ';
      }

      std::cout << '\n';
    }
  }
}

bool HasChoice(const Ballots& ballots) {
  for (const auto& one_vote: ballots) {
    if (!one_vote.empty()) {
      return true;
    }
  }

  return false;
}

bool HasChoiceBesidesCandidates(const Ballots& ballots,
                                const std::vector<int>& candidate_index) {
  for (const auto& one_vote: ballots) {
    if (!one_vote.empty() &&
        std::find(candidate_index.cbegin(), candidate_index.cend(),
                   one_vote.front()) == candidate_index.cend()) {
      return true;
    }
  }

  return false;
}

std::vector<int> FindLeast(std::unordered_map<int, int>& vote) {
  std::vector<int> least_indices;
  if (!vote.empty()) {
    int least = std::numeric_limits<int>::max();
    for (const auto& one : vote) {
      if (one.second < least) {
        least_indices.clear();
        least_indices.push_back(one.first);
        least = one.second;
      } else if (one.second == least) {
        least_indices.push_back(one.first);
      }
    }
  }

  return least_indices;
}

Results Process(const Cases& cases) {
  Results results;
  for (OneCase one_case : cases) {
    auto& [candidates, ballots] = one_case;  // C++17 structured binding
    ResultOneCase result_one_case;
    while (HasChoice(ballots) && result_one_case.empty()) {
      std::unordered_map<int, int> vote_caculate;
      for (auto& one_ballot : ballots) {
        if (!one_ballot.empty()) {
          ++vote_caculate[one_ballot.front() - 1];
        }
      }

      int vote_count = std::accumulate(
          vote_caculate.begin(), vote_caculate.end(), 0,
          [](int sum, const auto& a) {
            return sum + a.second;
          });
      int smallest = vote_count;
      for (const auto& per_vote: vote_caculate) {
        if (per_vote.second / vote_count > 0.5) {
          result_one_case.push_back(candidates[per_vote.first]);
        }
      }

      if (result_one_case.empty()) {
        std::vector<int> least_indices = FindLeast(vote_caculate);
        if (HasChoiceBesidesCandidates(ballots, least_indices)) {
          for (auto& one_vote : ballots) {
            for (const auto index: least_indices) {
              if (!one_vote.empty() && one_vote.front() - 1 == index) {
                one_vote.pop_front();
                break;
              }
            }
          }
        } else {
          for (auto index : least_indices) {
            result_one_case.push_back(candidates[index]);
          }

          for (auto& one_vote : ballots) {
            if (!one_vote.empty()) {
              one_vote.pop_front();
            }
          }
        }
      }
    }

    results.push_back(result_one_case);
  }

  return results;
}

void OutputResults(const Results& results) {
  for (auto result : results) {
    for (auto name : result) {
      std::cout << name << '\n';
    }

    std::cout << '\n';
  }
}
