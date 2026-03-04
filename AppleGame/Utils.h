#pragma once

#include <vector>

namespace ApplesGame
{
    struct LeaderboardEntry;

    namespace Utils
    {
        void SortByScoreDescending(std::vector<LeaderboardEntry>& leaderboard);
    }
}