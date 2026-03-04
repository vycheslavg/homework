#include "Utils.h"
#include "Leaderboard.h"
#include <utility>

namespace ApplesGame
{
    namespace Utils
    {
        void SortByScoreDescending(std::vector<LeaderboardEntry>& leaderboard)
        {
            for (size_t i = 0; i < leaderboard.size(); ++i)
            {
                for (size_t j = 0; j < leaderboard.size() - 1; ++j)
                {
                    if (leaderboard[j].score < leaderboard[j + 1].score)
                    {
                        std::swap(leaderboard[j], leaderboard[j + 1]);
                    }
                }
            }
        }
    }
}