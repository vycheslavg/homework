#include "Game.h"
#include <cassert>

namespace ApplesGame
{

    void GenerateLeaderboard(Game& game)
    {
        game.leaderboard.clear();

        std::vector<std::string> names =
        {
            "Alice","Bob","Carol","Dave",
            "Eve","Frank","Grace","Heidi"
        };

        int numPlayers = 7;

        for (int i = 0; i < numPlayers; ++i)
        {
            LeaderboardEntry entry;
            entry.name = names[i];
            entry.score = rand() % 150 + 10;
            game.leaderboard.push_back(entry);
        }
    }

    void UpdatePlayerInLeaderboard(Game& game)
    {
        bool found = false;

        for (auto& entry : game.leaderboard)
        {
            if (entry.name == "Player")
            {
                entry.score = game.numEatenApples;
                found = true;
                break;
            }
        }

        if (!found)
        {
            LeaderboardEntry playerEntry;
            playerEntry.name = "Player";
            playerEntry.score = game.numEatenApples;
            game.leaderboard.push_back(playerEntry);
        }
    }

    void SortLeaderboard(Game& game)
    {
        for (size_t i = 0; i < game.leaderboard.size(); ++i)
        {
            for (size_t j = 0; j < game.leaderboard.size() - 1; ++j)
            {
                if (game.leaderboard[j].score < game.leaderboard[j + 1].score)
                {
                    std::swap(game.leaderboard[j], game.leaderboard[j + 1]);
                }
            }
        }
    }

    void UpdateLeaderboardText(Game& game)
    {
        std::string text = "===== LEADERBOARD =====\n";

        for (size_t i = 0; i < game.leaderboard.size(); ++i)
        {
            text += std::to_string(i + 1) + ". ";
            text += game.leaderboard[i].name;

            int dots = 15 - game.leaderboard[i].name.length();
            for (int d = 0; d < dots; ++d)
                text += ".";

            text += " " + std::to_string(game.leaderboard[i].score);
            text += "\n";
        }

        text += "======================";

        game.leaderboardText.setString(text);
    }


    void StartPlayingState(Game& game)
    {
        SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
        SetPlayerSpeed(game.player, game.currentSpeed);
        SetPlayerDirection(game.player, PlayerDirection::Right);

        for (int i = 0; i < game.currentNumApples; ++i)
            SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));

        for (int i = 0; i < NUM_ROCKS; ++i)
            SetRockPosition(game.rocks[i], GetRandomPositionInRectangle(game.screenRect));

        game.numEatenApples = 0;
        game.isGameFinished = false;
        game.timeSinceGameFinish = 0;
        game.scoreText.setString("Apples eaten: 0");
    }

    void UpdatePlayingState(Game& game, float deltaTime)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            SetPlayerDirection(game.player, PlayerDirection::Right);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            SetPlayerDirection(game.player, PlayerDirection::Up);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            SetPlayerDirection(game.player, PlayerDirection::Left);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            SetPlayerDirection(game.player, PlayerDirection::Down);

        UpdatePlayer(game.player, deltaTime);

        for (int i = 0; i < game.currentNumApples; ++i)
        {
            if (DoShapesCollide(GetPlayerCollider(game.player), GetAppleCollider(game.apples[i])))
            {
                SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
                ++game.numEatenApples;
                SetPlayerSpeed(game.player, GetPlayerSpeed(game.player) + ACCELERATION);
                game.eatAppleSound.play();
                game.scoreText.setString("Apples eaten: " + std::to_string(game.numEatenApples));
            }
        }

        for (int i = 0; i < NUM_ROCKS; ++i)
            if (DoShapesCollide(GetPlayerCollider(game.player), GetRockCollider(game.rocks[i])))
                StartGameoverState(game);

        if (!DoShapesCollide(GetPlayerCollider(game.player), game.screenRect))
            StartGameoverState(game);
    }

    void StartGameoverState(Game& game)
    {
        game.isGameFinished = true;
        game.timeSinceGameFinish = 0.f;
        game.gameOverSound.play();

        game.gameOverScoreText.setString("Your score: " + std::to_string(game.numEatenApples));

        UpdatePlayerInLeaderboard(game);
        SortLeaderboard(game);
        UpdateLeaderboardText(game);
    }

    void UpdateGameoverState(Game& game, float deltaTime)
    {
        if (game.timeSinceGameFinish <= PAUSE_LENGTH)
        {
            game.timeSinceGameFinish += deltaTime;
            game.background.setFillColor(sf::Color::Red);
        }
        else
        {
            game.background.setFillColor(sf::Color::Black);
            StartPlayingState(game);
        }
    }


    void InitGame(Game& game)
    {
        if (game.mod == Modes::Mods_1)
        {
            game.currentNumApples = 20;
            game.currentSpeed = 150;
        }
        else if (game.mod == Modes::Mods_2)
        {
            game.currentNumApples = 20;
            game.currentSpeed = 150;
        }
        else if (game.mod == Modes::Mods_3)
        {
            game.currentNumApples = 100;
            game.currentSpeed = 200;
        }

        game.apples = new Apple[game.currentNumApples];

        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));
        assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
        assert(game.gameOverSoundBuffer.loadFromFile(RESOURCES_PATH + "\\Death.wav"));
        assert(game.font.loadFromFile(RESOURCES_PATH + "\\Fonts\\Roboto-Bold.ttf"));

        game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

        InitPlayer(game.player, game);

        for (int i = 0; i < game.currentNumApples; ++i)
            InitApple(game.apples[i], game);

        for (int i = 0; i < NUM_ROCKS; ++i)
            InitRock(game.rocks[i], game);

        game.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.background.setFillColor(sf::Color::Black);

        game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
        game.gameOverSound.setBuffer(game.gameOverSoundBuffer);

        game.scoreText.setFont(game.font);
        game.scoreText.setCharacterSize(20);
        game.scoreText.setFillColor(sf::Color::White);
        game.scoreText.setPosition(20.f, 10.f);

        game.controlsHintText.setFont(game.font);
        game.controlsHintText.setCharacterSize(20);
        game.controlsHintText.setFillColor(sf::Color::White);
        game.controlsHintText.setString("Use arrows to move, ESC to exit");
        game.controlsHintText.setPosition(SCREEN_WIDTH - 350.f, 10.f);

        game.gameOverText.setFont(game.font);
        game.gameOverText.setCharacterSize(100);
        game.gameOverText.setFillColor(sf::Color::White);
        game.gameOverText.setString("Game Over");
        game.gameOverText.setPosition(SCREEN_WIDTH / 2.f - 200.f,
            SCREEN_HEIGHT / 2.f - 120.f);

        game.gameOverScoreText.setFont(game.font);
        game.gameOverScoreText.setCharacterSize(30);
        game.gameOverScoreText.setFillColor(sf::Color::White);
        game.gameOverScoreText.setPosition(SCREEN_WIDTH / 2.f - 100.f,
            SCREEN_HEIGHT / 2.f - 20.f);

        GenerateLeaderboard(game);

        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(20);
        game.leaderboardText.setFillColor(sf::Color::White);
        game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - 200.f,
            SCREEN_HEIGHT / 2.f + 30.f);

        StartPlayingState(game);
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        if (!game.isGameFinished)
            UpdatePlayingState(game, deltaTime);
        else
            UpdateGameoverState(game, deltaTime);
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.background);

        DrawPlayer(game.player, window);

        for (int i = 0; i < game.currentNumApples; ++i)
            DrawApple(game.apples[i], window);

        for (int i = 0; i < NUM_ROCKS; ++i)
            DrawRock(game.rocks[i], window);

        if (!game.isGameFinished)
        {
            window.draw(game.scoreText);
            window.draw(game.controlsHintText);
        }
        else
        {
            window.draw(game.gameOverText);
            window.draw(game.gameOverScoreText);
            window.draw(game.leaderboardText);
        }
    }

    void DeinializeGame(Game& game)
    {
        delete[] game.apples;
    }

}
