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
#include <set>
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

  return voting_cases;
}

// 依然要计票，并且仍有票可计数。
bool HasChoice(const Ballots& ballots) {
  for (const auto& one_vote: ballots) {
    if (!one_vote.empty()) {
      return true;
    }
  }

  return false;
}

// 剩下的候选人是否打平。
bool IsCandidatesLeftTies(const std::set<size_t>& candidates_left,
                          const std::vector<size_t>& least_votes_candidates) {
  // 剩下候选人中有人不是得票最少的候选人之一。
  for (const auto i : candidates_left) {
    if (std::find(least_votes_candidates.cbegin(), least_votes_candidates.cend(), i)
        == least_votes_candidates.cend()) {
      return false;
    }
  }

  return true;
}

// 找到得票最少的候选人的索引。
std::vector<size_t> FindLeastVotesCandidatesIndex(std::unordered_map<size_t, int>& vote) {
  std::vector<size_t> least_indices;
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

  return least_indices;
}

void RemoveLeastVotesCandidates(std::set<size_t>& candidates_left,
                                const std::vector<size_t>& least_votes_candidates) {
  for (auto i : least_votes_candidates) {
    candidates_left.erase(i);
  }
}

std::vector<size_t> ProcessOneCaseReal(Ballots& ballots, const size_t candidates_num) {
  // 剩下的候选人索引集合
  std::set<size_t> candidates_left;
  for (size_t i = 0; i < candidates_num; ++i) {
    candidates_left.insert(i);
  }

  std::vector<size_t> result_one_case;
  while (HasChoice(ballots) && result_one_case.empty()) {
    std::unordered_map<size_t, int> vote_caculate;
    // 对剩下候选人计票，避免未统计此轮没得票候选人
    for (const auto index : candidates_left) {
      vote_caculate[index] = 0;
    }

    int vote_total = 0;
    for (const auto& one_ballot : ballots) {
      if (!one_ballot.empty()) {
        ++vote_caculate[one_ballot.front() - 1];
        ++vote_total;
      }
    }

    for (const auto& per_vote: vote_caculate) {
      if (per_vote.second / vote_total > 0.5) {
        result_one_case.push_back(per_vote.first);
      }
    }

    if (result_one_case.empty()) {
      std::vector<size_t> least_indices
          = FindLeastVotesCandidatesIndex(vote_caculate);
      if (IsCandidatesLeftTies(candidates_left, least_indices)) {
        for (auto index : least_indices) {
          result_one_case.push_back(index);
        }
      } else {
        // 剔除得票最少的候选人 
        RemoveLeastVotesCandidates(candidates_left, least_indices);
        // 从选票池中删除所有已经出局的候选人的选票
        for (auto& one_vote : ballots) {
          one_vote.remove_if([&candidates_left](int x) {
            return candidates_left.find(x-1) == candidates_left.end();
          });
        }
      }
    }
  }

  return result_one_case;
}

ResultOneCase ProcessOneCase(Ballots& ballots, const Candidates& candidates) {
  ResultOneCase result_one_case;
  std::vector<size_t> indices = ProcessOneCaseReal(ballots, candidates.size());
  for (auto i : indices) {
    result_one_case.push_back(candidates[i]);
  }

  return result_one_case;
}

Results Process(const Cases& cases) {
  Results results;
  for (OneCase one_case : cases) {
    auto& [candidates, ballots] = one_case;  // C++17 structured binding
    ResultOneCase result_one_case = ProcessOneCase(ballots, candidates);
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
