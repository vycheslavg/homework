#include "Leaderboard.h"
#include "Utils.h"

namespace ApplesGame
{

    void GenerateLeaderboard(std::vector<LeaderboardEntry>& leaderboard)
    {
        leaderboard.clear();

        std::vector<std::string> names =
        {
            "Alice","Bob","Carol","Dave",
            "Eve","Frank","Grace","Heidi"
        };

        int count = 7;

        for (int i = 0; i < count; ++i)
        {
            LeaderboardEntry entry;
            entry.name = names[i];
            entry.score = rand() % 150 + 10;
            leaderboard.push_back(entry);
        }
    }

    void UpdatePlayerScore(std::vector<LeaderboardEntry>& leaderboard,
        const std::string& playerName,
        int score)
    {
        bool found = false;

        for (auto& entry : leaderboard)
        {
            if (entry.name == playerName)
            {
                entry.score = score;
                found = true;
                break;
            }
        }

        if (!found)
        {
            leaderboard.push_back({ playerName, score });
        }
    }

    void SortLeaderboard(std::vector<LeaderboardEntry>& leaderboard)
    {
        Utils::SortByScoreDescending(leaderboard);
    }

    void BuildLeaderboardText(const std::vector<LeaderboardEntry>& leaderboard,
        sf::Text& text)
    {
        std::string result = "===== LEADERBOARD =====\n";

        for (size_t i = 0; i < leaderboard.size(); ++i)
        {
            result += std::to_string(i + 1) + ". ";
            result += leaderboard[i].name;

            int dots = 15 - leaderboard[i].name.length();
            for (int d = 0; d < dots; ++d)
                result += ".";

            result += " " + std::to_string(leaderboard[i].score);
            result += "\n";
        }

        result += "=====================";

        text.setString(result);
    }

}