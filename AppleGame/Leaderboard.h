#pragma once
#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
    struct LeaderboardEntry
    {
        std::string name;
        int score;
    };

    void GenerateLeaderboard(std::vector<LeaderboardEntry>& leaderboard);

    void UpdatePlayerScore(std::vector<LeaderboardEntry>& leaderboard,
        const std::string& playerName,
        int score);

    void SortLeaderboard(std::vector<LeaderboardEntry>& leaderboard);

    void BuildLeaderboardText(const std::vector<LeaderboardEntry>& leaderboard,
        sf::Text& text);
}