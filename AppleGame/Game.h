#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
    enum class Modes : uint8_t
    {
        None = 0,
        Mods_1 = 1 << 0,  
        Mods_2 = 1 << 1,  
        Mods_3 = 1 << 2   
    };

    inline Modes operator|(Modes a, Modes b)
    {
        return static_cast<Modes>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
            );
    }

    inline Modes operator&(Modes a, Modes b)
    {
        return static_cast<Modes>(
            static_cast<uint8_t>(a) & static_cast<uint8_t>(b)
            );
    }

    inline Modes& operator|=(Modes& a, Modes b)
    {
        a = a | b;
        return a;
    }

    inline bool HasFlag(Modes value, Modes flag)
    {
        return (value & flag) == flag;
    }
   

    struct Game
    {
        Modes mod = Modes::None;   

        Rectangle screenRect;

        Player player;
        Apple* apples;
        Rock rocks[NUM_ROCKS];

        int numEatenApples = 0;
        bool isGameFinished = false;
        float timeSinceGameFinish = 0.f;
        int currentNumApples = NUM_APPLES;
        float currentSpeed = INITIAL_SPEED;

        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;
        sf::SoundBuffer eatAppleSoundBuffer;
        sf::SoundBuffer gameOverSoundBuffer;

        sf::Sound eatAppleSound;
        sf::Sound gameOverSound;
        sf::RectangleShape background;

        sf::Font font;
        sf::Text scoreText;
        sf::Text controlsHintText;
        sf::Text gameOverText;
        sf::Text gameOverScoreText;
    };

    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void DeinializeGame(Game& game);

    void StartPlayingState(Game& game);
    void UpdatePlayingState(Game& game, float deltaTime);

    void StartGameoverState(Game& game);
    void UpdateGameoverState(Game& game, float deltaTime);
}