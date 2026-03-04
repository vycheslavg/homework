#include "Game.h"
#include "Leaderboard.h"
#include <cassert>

namespace ApplesGame
{

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

        UpdatePlayerScore(game.leaderboard, "Player", game.numEatenApples);
        SortLeaderboard(game.leaderboard);
        BuildLeaderboardText(game.leaderboard, game.leaderboardText);
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

        GenerateLeaderboard(game.leaderboard);

        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(20);
        game.leaderboardText.setFillColor(sf::Color::White);
        game.leaderboardText.setPosition(
            SCREEN_WIDTH / 2.f - 200.f,
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
