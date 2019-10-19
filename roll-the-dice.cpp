/** \file roll-the-dice.cpp
 *  \author Copyright 2019, Matthew Gretton-Dann
 *
 * spdx:Apache-2.0
 *
 * Experimentally (because its quicker than doing the maths) generate a CSV
 * file that gives a score of how many rolls of a dice it takes to reach a
 * particular score.
 */

#include <cassert>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <random>

/// How many sides does the dice have?
#define DICE_SIDES 6
/// What's the target score we want
#define TARGET 120
/// How many runs to aim for.
#define NUM_RUNS 100000000

using IntList = std::list<int>;
using IntMap = std::map<int, int>;

int main(int argc, char** argv) {
  // Basic algorithm:
  // Build a list of dice rolls in the list rolls, keeping track of its length
  // (the number of rolls) and the sum of the rolls (score).
  // When the score is greater than TARGET increment the tally for the current
  // length.
  // Repeat until we have done NUM_RUNS runs.
  // As an optimisation - because the rolls are independent assume that we can
  // pop the front off the list of rolls when we've met the target and use
  // that shortened list as a roll count, without disturbing the distribution
  // results.
  int runs{0};    ///< Number of runs completed
  int score{0};   ///< Current score (sum(rolls.begin(), rolls.end()))
  int len{0};     ///< Length of rolls
  IntList rolls;  ///< List of dice rolls
  IntMap tally;   ///< Tally of number of rolls -> count
  /// Set up the dist that will act as the dice:
  auto seed{std::chrono::system_clock::now().time_since_epoch().count()};
  std::default_random_engine generator{seed};
  std::uniform_int_distribution<int> dist{1, DICE_SIDES};

  while (runs < NUM_RUNS) {
    assert(score < TARGET);

    // Roll the dice
    int roll{dist(generator)};

    // Record the roll
    rolls.push_back(roll);
    ++len;
    score += roll;

    while (score >= TARGET) {
      // Record the count for the target.
      // This is a while loop because the roll we've just added might push us
      // over TARGET by more than the roll at the front we will remove.
      auto it{tally.find(len)};
      if (it == tally.end()) {
        tally.insert(std::pair<int, int>(len, 1));
      } else {
        ++it->second;
      }
      ++runs;

      score -= rolls.front();
      --len;
      rolls.pop_front();
    }
  }

  // Output results
  std::cout << "\"Number of rolls\",\"Count\"\n";
  for (auto it : tally) {
    std::cout << it.first << "," << it.second << "\n";
  }

  // Exit with success
  return 0;
}

