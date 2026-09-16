#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <cmath>
#include <vector>

#include "Arenero.hpp"
#include "Cama.hpp"
#include "Comida.hpp"
#include "Dueno.hpp"
#include "Gato.hpp"
#include "GatochiImplementaciones.hpp"
#include "Juego.hpp"
#include "Veterinario.hpp"

int EjecutarJuego()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gatochi");
    window.setKeyRepeatEnabled(false);
    Dueno dueno("Jugador");
    Gato gato("Gatochi", 1);
    Comida comida("comida", 3.0f);
    Arenero arenero(true, "derecha");
    Cama cama;
    Veterinario veterinario("Veterinario", "Medicina felina");

    sf::Texture coverTexture;
    if (!coverTexture.loadFromFile("assets/Images/gatochi.jpg"))
    {
        return 1;
    }

    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("assets/Images/Fondo Principal.jpg"))
    {
        return 1;
    }

    sf::Texture catIdleTexture;
    if (!catIdleTexture.loadFromFile("assets/Images/Gato inactivo.png"))
    {
        return 1;
    }

    sf::Texture catRightTexture;
    if (!catRightTexture.loadFromFile("assets/Images/Gato Derecha.png"))
    {
        return 1;
    }

    sf::Texture catLeftTexture;
    if (!catLeftTexture.loadFromFile("assets/Images/Gato Izquierda.png"))
    {
        return 1;
    }

    sf::Texture energyTexture;
    if (!energyTexture.loadFromFile("assets/Images/Energia.png"))
    {
        return 1;
    }

    sf::Texture foodTexture;
    if (!foodTexture.loadFromFile("assets/Images/Comida.png"))
    {
        return 1;
    }

    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("assets/Images/Game Over.jpg"))
    {
        return 1;
    }

    sf::Texture registrationTexture;
    if (!registrationTexture.loadFromFile("assets/Images/Registro.jpg"))
    {
        return 1;
    }

    sf::Texture labyrinthTexture;
    if (!labyrinthTexture.loadFromFile("assets/Images/Laberinto.jpg"))
    {
        return 1;
    }

    sf::Texture catUpTexture;
    if (!catUpTexture.loadFromFile("assets/Images/Caminado hacia arriba.png"))
    {
        return 1;
    }

    sf::Texture catDownTexture;
    if (!catDownTexture.loadFromFile("assets/Images/Caminado hacia abajo.png"))
    {
        return 1;
    }

    auto fitSpriteToWindow = [](sf::Sprite& sprite, const sf::RenderWindow& window)
    {
        const sf::Vector2u spriteSize = sprite.getTexture()->getSize();
        const float scaleX = static_cast<float>(window.getSize().x) / static_cast<float>(spriteSize.x);
        const float scaleY = static_cast<float>(window.getSize().y) / static_cast<float>(spriteSize.y);
        const float scale = std::min(scaleX, scaleY);
        sprite.setScale(scale, scale);
        sprite.setPosition(
            (static_cast<float>(window.getSize().x) - static_cast<float>(spriteSize.x) * scale) / 2.0f,
            (static_cast<float>(window.getSize().y) - static_cast<float>(spriteSize.y) * scale) / 2.0f);
    };

    sf::Sprite cover(coverTexture);
    sf::Sprite background(mainTexture);
    sf::Sprite cat(catIdleTexture);
    sf::Sprite gameOverBackground(gameOverTexture);
    sf::Sprite registrationBackground(registrationTexture);
    sf::Sprite labyrinthBackground(labyrinthTexture);
    fitSpriteToWindow(cover, window);
    fitSpriteToWindow(background, window);
    fitSpriteToWindow(gameOverBackground, window);
    fitSpriteToWindow(registrationBackground, window);
    fitSpriteToWindow(labyrinthBackground, window);

    const sf::FloatRect labyrinthBounds = labyrinthBackground.getGlobalBounds();
    const std::vector<std::string> labyrinthMap = {
        "0000000000000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000000000000000000000",
        "0111111111111111111111111111111111111111111111111111111111111111111110",
        "0111111111111111111111111111111111111111111111111111111111111111111110",
        "0111111111111111111111111111111111111111111111111111111111111111111110",
        "0111110000011111111111110000001110111011100000001111110111111000001110",
        "0111110000011111101111110000001110111011100000001111110111111000001110",
        "0111111111111111101111110000001110111011100000001111110111111111111110",
        "0111111111111111101111111111111111111111111111111111110111111111111110",
        "0111111111111111101111111111111111111111111111111111110111111111111110",
        "0111000111111111111111111111111111111111111111111111110111111111111110",
        "0111111111110111111110111000000000000000000001111001111111011110111110",
        "0111111111110111111110111001111111111111111001111001111111011111111110",
        "0111111111110111111110111001111111111111111001111001111111011111111110",
        "0000000011110111111110111000111111111111111001111001111111011111111110",
        "1111110011110111000000111001111111111111111001111000000111011110000000",
        "1111110011110111111111111001111111111111111001111000000111011110111111",
        "1111110011110111111111111001111111111111111001111111111111011110111111",
        "1111110011110111111111111001111111111111111001111111111111011110111111",
        "0000000011110111111111111001111111111111111001111111111111011110111111",
        "1111111111111111111111111111111111111111111111111111111111111110000000",
        "1111111111111111111110111111111111111111111111110111111111111111111111",
        "1111111111111111111111111111111111111111111111111111111111111111111111",
        "1111111111111111111111111001111111111111111001111111111111111111111111",
        "0000000011110111111111111001111111111111111001111111111111011110000000",
        "1111111011110111111111111001111111111111111001111111111111011110111111",
        "1111111011110111100000111001111111111111111001110000011111011110111111",
        "1111111011110111111111111001111111111111111001110000011111011110111111",
        "1111111011110111111111111000000001111100000001111111111111011110111111",
        "0000000011110111111111111101111111111111111011111111111111011110000000",
        "0111111111110111111111111111111111111111111111111111111111011111111110",
        "0111111111110111111111111111111111111111111111111111111111011111111110",
        "0111111111111111111101111111111111111111111111111111111111011111111110",
        "0111110011111111111101111111111111111111111111111101111111111110111110",
        "0111111111111111111101111101111111111111111111111101111111111111111110",
        "0111111111111111111101111101111111111111111110111101111111111111111110",
        "0111111111110111011101111101111000000000011110111101111011111111111110",
        "0111100011110111011100111100000111110111111110111101111011111110001110",
        "0111111111110000011111111100000111110111111110111101111000011111111110",
        "0111111111110000011111111111111111110111111110111001111000011111111110",
        "0111111111111111111111111111111111110111111111111111111111111111111110",
        "0111111111111111111111111111111111110111111111111111111111111111111110",
        "0111111111111111111111111111111111111111111111111111111111111111111110",
        "0000000000000000000000000000000001111111000000000000000000000000000000",
        "0000000000000000000000000000000001111111000000000000000000000000000000",
    };
    const int labyrinthRows = static_cast<int>(labyrinthMap.size());
    const int labyrinthColumns = static_cast<int>(labyrinthMap.front().size());
    auto labyrinthCollides = [&](const sf::FloatRect& bounds)
    {
        const float leftPixel = (bounds.left - labyrinthBounds.left) /
            labyrinthBounds.width * labyrinthColumns;
        const float topPixel = (bounds.top - labyrinthBounds.top) /
            labyrinthBounds.height * labyrinthRows;
        const float rightPixel = (bounds.left + bounds.width - labyrinthBounds.left) /
            labyrinthBounds.width * labyrinthColumns;
        const float bottomPixel = (bounds.top + bounds.height - labyrinthBounds.top) /
            labyrinthBounds.height * labyrinthRows;
        if (rightPixel < 0.0f || bottomPixel < 0.0f ||
            leftPixel >= labyrinthColumns || topPixel >= labyrinthRows)
        {
            return true;
        }
        const int left = std::max(0, static_cast<int>(std::floor(leftPixel)));
        const int top = std::max(0, static_cast<int>(std::floor(topPixel)));
        const int right = std::min(
            labyrinthColumns - 1, static_cast<int>(std::ceil(rightPixel)));
        const int bottom = std::min(
            labyrinthRows - 1, static_cast<int>(std::ceil(bottomPixel)));
        for (int row = top; row <= bottom; ++row)
        {
            for (int column = left; column <= right; ++column)
            {
                if (labyrinthMap[row][column] == '0')
                {
                    return true;
                }
            }
        }
        return false;
    };

    const sf::Vector2u catSize = catIdleTexture.getSize();
    const sf::Vector2u energySheetSize = energyTexture.getSize();
    const int energyFrameWidth = static_cast<int>(energySheetSize.x / 2);
    const int energyFrameHeight = static_cast<int>(energySheetSize.y);
    const sf::Vector2u catRightSheetSize = catRightTexture.getSize();
    const sf::Vector2u catLeftSheetSize = catLeftTexture.getSize();
    const int catRightFrameCount = 4;
    const int catRightFrameWidth = static_cast<int>(catRightSheetSize.x / catRightFrameCount);
    const int catRightFrameHeight = static_cast<int>(catRightSheetSize.y);
    const int catLeftFrameCount = 4;
    const int catLeftFrameWidth = static_cast<int>(catLeftSheetSize.x / catLeftFrameCount);
    const int catLeftFrameHeight = static_cast<int>(catLeftSheetSize.y);
    const sf::Vector2u catUpSheetSize = catUpTexture.getSize();
    const sf::Vector2u catDownSheetSize = catDownTexture.getSize();
    const int catUpFrameCount = 4;
    const int catDownFrameCount = 4;
    const int catUpFrameWidth = static_cast<int>(catUpSheetSize.x / catUpFrameCount);
    const int catUpFrameHeight = static_cast<int>(catUpSheetSize.y);
    const int catDownFrameWidth = static_cast<int>(catDownSheetSize.x / catDownFrameCount);
    const int catDownFrameHeight = static_cast<int>(catDownSheetSize.y);
    const sf::FloatRect backgroundBounds = background.getGlobalBounds();
    const float catScale = std::min(
        (backgroundBounds.width * 0.16f) / static_cast<float>(catSize.x),
        (backgroundBounds.height * 0.30f) / static_cast<float>(catSize.y));
    const float catIdleVisualHeight = static_cast<float>(catSize.y) * catScale;
    const float moveScaleBoost = 3.0f;
    const float catRightScale =
        (catIdleVisualHeight / static_cast<float>(catRightFrameHeight)) * moveScaleBoost;
    const float catLeftScale =
        (catIdleVisualHeight / static_cast<float>(catLeftFrameHeight)) * moveScaleBoost;
    const float catGroundY = backgroundBounds.top + backgroundBounds.height * 0.87f;
    const float rightAnimYOffset = 130.0f;
    const float leftAnimYOffset = 130.0f;
    const float catRightGroundY = catGroundY + rightAnimYOffset;
    const float catLeftGroundY = catGroundY + leftAnimYOffset;
    const float labyrinthCatScale = std::min(
        (labyrinthBounds.width * 0.11f) / static_cast<float>(catSize.x),
        (labyrinthBounds.height * 0.22f) / static_cast<float>(catSize.y));
    const float labyrinthCatIdleScale = labyrinthCatScale * 0.49f;
    const float labyrinthIdleVisualYOffset = -35.0f;
    const float labyrinthCatVisualHeight =
        static_cast<float>(catSize.y) * labyrinthCatScale;
    const float labyrinthCatRightScale =
        (labyrinthCatVisualHeight / static_cast<float>(catRightFrameHeight)) * 1.35f;
    const float labyrinthCatLeftScale =
        (labyrinthCatVisualHeight / static_cast<float>(catLeftFrameHeight)) * 1.35f;
    const float labyrinthCatUpScale =
        (labyrinthCatVisualHeight / static_cast<float>(catUpFrameHeight)) * 1.35f;
    const float labyrinthCatDownScale =
        (labyrinthCatVisualHeight / static_cast<float>(catDownFrameHeight)) * 1.35f;
    auto labyrinthCollisionBounds = [&](const sf::Sprite& sprite)
    {
        const sf::FloatRect visualBounds = sprite.getGlobalBounds();
        const float width = labyrinthBounds.width /
            static_cast<float>(labyrinthColumns);
        const float height = labyrinthBounds.height /
            static_cast<float>(labyrinthRows);
        return sf::FloatRect(
            visualBounds.left + (visualBounds.width - width) / 2.0f,
            visualBounds.top + (visualBounds.height - height) / 2.0f,
            width, height);
    };
    cat.setOrigin(0.0f, cat.getLocalBounds().height);
    cat.setScale(catScale, catScale);
    cat.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.47f,
        catGroundY);

    sf::Sprite energySprites[3];
    const float energyScale = 0.10f;
    const float energyMargin = 20.0f;
    for (int index = 0; index < 3; ++index)
    {
        energySprites[index].setTexture(energyTexture);
        energySprites[index].setTextureRect(
            sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
        energySprites[index].setScale(energyScale, energyScale);
        energySprites[index].setPosition(
            static_cast<float>(window.getSize().x) - energyMargin -
                energyFrameWidth * energyScale * static_cast<float>(3 - index),
            energyMargin);
    }

    sf::Font titleFont;
    if (!titleFont.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        if (!titleFont.loadFromFile("C:/Windows/Fonts/calibri.ttf"))
        {
            if (!titleFont.loadFromFile("assets/Fonts/default.ttf"))
            {
                return 1;
            }
        }
    }

    comida.ConfigurarInterfaz(foodTexture, titleFont, backgroundBounds);
    arenero.ConfigurarInterfaz(titleFont, backgroundBounds);
    cama.ConfigurarInterfaz(titleFont, backgroundBounds);

    sf::Text pressText;
    pressText.setFont(titleFont);
    pressText.setString("Presione espacio para jugar");
    pressText.setCharacterSize(std::max(22u, static_cast<unsigned int>(window.getSize().y / 22u)));
    pressText.setFillColor(sf::Color::White);
    pressText.setStyle(sf::Text::Bold);
    pressText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        cover.getPosition().y + cover.getGlobalBounds().height + 26.0f);
    pressText.setOrigin(
        pressText.getLocalBounds().left + pressText.getLocalBounds().width / 2.0f,
        pressText.getLocalBounds().top + pressText.getLocalBounds().height / 2.0f);

    sf::Text foodText;
    foodText.setFont(titleFont);
    foodText.setString("ve a comer");
    foodText.setCharacterSize(std::max(26u, static_cast<unsigned int>(window.getSize().y / 18u)));
    foodText.setFillColor(sf::Color::White);
    foodText.setStyle(sf::Text::Bold);
    foodText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        18.0f);
    foodText.setOrigin(
        foodText.getLocalBounds().left + foodText.getLocalBounds().width / 2.0f,
        foodText.getLocalBounds().top);

    sf::Text sickTimerText;
    sickTimerText.setFont(titleFont);
    sickTimerText.setCharacterSize(22);
    sickTimerText.setFillColor(sf::Color(255, 210, 210));
    sickTimerText.setStyle(sf::Text::Bold);
    sickTimerText.setPosition(400.0f, 18.0f);
    sickTimerText.setOrigin(
        sickTimerText.getLocalBounds().left + sickTimerText.getLocalBounds().width / 2.0f,
        sickTimerText.getLocalBounds().top);

    sf::RectangleShape veterinarianOption(sf::Vector2f(300.0f, 46.0f));
    veterinarianOption.setPosition(250.0f, 52.0f);
    veterinarianOption.setFillColor(sf::Color(80, 30, 35, 230));
    veterinarianOption.setOutlineThickness(2.0f);
    veterinarianOption.setOutlineColor(sf::Color(255, 180, 180));

    sf::Text veterinarianOptionText;
    veterinarianOptionText.setFont(titleFont);
    veterinarianOptionText.setString("Llevar al veterinario (V)");
    veterinarianOptionText.setCharacterSize(20);
    veterinarianOptionText.setFillColor(sf::Color::White);
    veterinarianOptionText.setStyle(sf::Text::Bold);
    veterinarianOptionText.setPosition(400.0f, 75.0f);
    veterinarianOptionText.setOrigin(
        veterinarianOptionText.getLocalBounds().left + veterinarianOptionText.getLocalBounds().width / 2.0f,
        veterinarianOptionText.getLocalBounds().top + veterinarianOptionText.getLocalBounds().height / 2.0f);

    sf::Text labyrinthOptionText;
    labyrinthOptionText.setFont(titleFont);
    labyrinthOptionText.setString("Presiona P para ir a jugar");
    labyrinthOptionText.setCharacterSize(22);
    labyrinthOptionText.setFillColor(sf::Color::White);
    labyrinthOptionText.setStyle(sf::Text::Bold);
    labyrinthOptionText.setPosition(400.0f, 112.0f);
    labyrinthOptionText.setOrigin(
        labyrinthOptionText.getLocalBounds().left + labyrinthOptionText.getLocalBounds().width / 2.0f,
        labyrinthOptionText.getLocalBounds().top);

    sf::RectangleShape sadOverlay(sf::Vector2f(
        static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    sadOverlay.setFillColor(sf::Color(35, 45, 75, 105));

    sf::Text registrationTitle;
    registrationTitle.setFont(titleFont);
    registrationTitle.setString("Registro del veterinario");
    registrationTitle.setCharacterSize(30);
    registrationTitle.setFillColor(sf::Color::White);
    registrationTitle.setStyle(sf::Text::Bold);
    registrationTitle.setPosition(400.0f, 70.0f);
    registrationTitle.setOrigin(
        registrationTitle.getLocalBounds().left + registrationTitle.getLocalBounds().width / 2.0f,
        registrationTitle.getLocalBounds().top);

    sf::Text registrationLabels[3];
    sf::Text registrationValues[3];
    const char* registrationLabelText[3] = {"Nombre", "Edad", "Genero"};
    for (int index = 0; index < 3; ++index)
    {
        registrationLabels[index].setFont(titleFont);
        registrationLabels[index].setString(registrationLabelText[index]);
        registrationLabels[index].setCharacterSize(22);
        registrationLabels[index].setFillColor(sf::Color::White);
        registrationLabels[index].setPosition(190.0f, 175.0f + index * 100.0f);

        registrationValues[index].setFont(titleFont);
        registrationValues[index].setCharacterSize(22);
        registrationValues[index].setFillColor(sf::Color::Black);
        registrationValues[index].setPosition(370.0f, 175.0f + index * 100.0f);
    }

    sf::RectangleShape registrationFields[3];
    for (int index = 0; index < 3; ++index)
    {
        registrationFields[index].setSize(sf::Vector2f(300.0f, 46.0f));
        registrationFields[index].setPosition(360.0f, 165.0f + index * 100.0f);
        registrationFields[index].setFillColor(sf::Color(255, 255, 255, 220));
        registrationFields[index].setOutlineThickness(3.0f);
        registrationFields[index].setOutlineColor(sf::Color::White);
    }

    sf::Text registrationInstruction;
    const std::string registrationInstructionDefault =
        "Completa los datos y presiona Enter";
    registrationInstruction.setFont(titleFont);
    registrationInstruction.setString(registrationInstructionDefault);
    registrationInstruction.setCharacterSize(20);
    registrationInstruction.setFillColor(sf::Color::White);
    registrationInstruction.setPosition(400.0f, 510.0f);
    registrationInstruction.setOrigin(
        registrationInstruction.getLocalBounds().left + registrationInstruction.getLocalBounds().width / 2.0f,
        registrationInstruction.getLocalBounds().top);

    sf::Text gameOverStateText;
    gameOverStateText.setFont(titleFont);
    gameOverStateText.setCharacterSize(24);
    gameOverStateText.setFillColor(sf::Color::White);
    gameOverStateText.setStyle(sf::Text::Bold);

    sf::RectangleShape restartOption(sf::Vector2f(300.0f, 54.0f));
    sf::RectangleShape reviveOption(sf::Vector2f(300.0f, 54.0f));
    restartOption.setPosition(250.0f, 435.0f);
    reviveOption.setPosition(250.0f, 500.0f);
    restartOption.setFillColor(sf::Color(30, 30, 30, 220));
    reviveOption.setFillColor(sf::Color(30, 30, 30, 220));
    restartOption.setOutlineThickness(3.0f);
    reviveOption.setOutlineThickness(3.0f);

    sf::Text restartText;
    restartText.setFont(titleFont);
    restartText.setString("volver a iniciar el juego");
    restartText.setCharacterSize(21);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(400.0f, 462.0f);
    restartText.setOrigin(
        restartText.getLocalBounds().left + restartText.getLocalBounds().width / 2.0f,
        restartText.getLocalBounds().top + restartText.getLocalBounds().height / 2.0f);

    sf::Text reviveText;
    reviveText.setFont(titleFont);
    reviveText.setString("Revivir");
    reviveText.setCharacterSize(21);
    reviveText.setFillColor(sf::Color::White);
    reviveText.setPosition(400.0f, 527.0f);
    reviveText.setOrigin(
        reviveText.getLocalBounds().left + reviveText.getLocalBounds().width / 2.0f,
        reviveText.getLocalBounds().top + reviveText.getLocalBounds().height / 2.0f);

    sf::Text veterinarianText;
    veterinarianText.setFont(titleFont);
    veterinarianText.setString("veterinario");
    veterinarianText.setCharacterSize(36);
    veterinarianText.setFillColor(sf::Color::White);
    veterinarianText.setStyle(sf::Text::Bold);
    veterinarianText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        static_cast<float>(window.getSize().y) / 2.0f);
    veterinarianText.setOrigin(
        veterinarianText.getLocalBounds().left + veterinarianText.getLocalBounds().width / 2.0f,
        veterinarianText.getLocalBounds().top + veterinarianText.getLocalBounds().height / 2.0f);

    sf::Text healthText;
    healthText.setFont(titleFont);
    healthText.setString("SANO");
    healthText.setCharacterSize(26);
    healthText.setFillColor(sf::Color::White);
    healthText.setStyle(sf::Text::Bold);
    healthText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        static_cast<float>(window.getSize().y) - 24.0f);
    healthText.setOrigin(
        healthText.getLocalBounds().left + healthText.getLocalBounds().width / 2.0f,
        healthText.getLocalBounds().top + healthText.getLocalBounds().height / 2.0f);

    sf::Music music;
    if (!music.openFromFile("assets/Music/Fondo.mp3"))
    {
        return 1;
    }
    music.setLoop(true);
    music.play();

    sf::Music gameOverMusic;
    if (!gameOverMusic.openFromFile("assets/Music/Game Over Song.mp3"))
    {
        return 1;
    }
    gameOverMusic.setLoop(true);

    sf::Clock transitionClock;
    sf::Clock frameClock;
    sf::Clock catAnimClock;
    sf::Clock healthTransitionClock;
    bool transitioning = false;
    bool started = false;
    bool registrationScreen = false;
    int selectedRegistrationField = 0;
    std::string registrationValuesText[3];
    int currentRightFrame = 0;
    int currentLeftFrame = 0;
    bool labyrinthIdleOffsetApplied = false;
    const float rightFrameDuration = 0.10f;
    const float leftFrameDuration = 0.10f;
    auto moveLabyrinthCat = [&](bool movingRight, bool movingLeft,
        bool movingUp, bool movingDown, float deltaTime)
    {
        const bool movingHorizontal = movingRight != movingLeft;
        const bool movingVertical = movingUp != movingDown;
        const sf::Texture* movementTexture = nullptr;
        int* currentFrame = nullptr;
        int frameWidth = 0;
        int frameHeight = 0;
        float movementScale = labyrinthCatScale;

        if (movingVertical)
        {
            movementTexture = movingUp ? &catUpTexture : &catDownTexture;
            currentFrame = movingUp ? &currentLeftFrame : &currentRightFrame;
            frameWidth = movingUp ? catUpFrameWidth : catDownFrameWidth;
            frameHeight = movingUp ? catUpFrameHeight : catDownFrameHeight;
            movementScale = movingUp ? labyrinthCatUpScale : labyrinthCatDownScale;
        }
        else if (movingHorizontal)
        {
            movementTexture = movingRight ? &catRightTexture : &catLeftTexture;
            currentFrame = movingRight ? &currentRightFrame : &currentLeftFrame;
            frameWidth = movingRight ? catRightFrameWidth : catLeftFrameWidth;
            frameHeight = movingRight ? catRightFrameHeight : catLeftFrameHeight;
            movementScale = movingRight ? labyrinthCatRightScale : labyrinthCatLeftScale;
        }

        if (movementTexture == nullptr)
        {
            if (cat.getTexture() != &catIdleTexture)
            {
                const sf::Vector2f position = cat.getPosition();
                cat.setTexture(catIdleTexture, true);
                cat.setOrigin(0.0f, cat.getLocalBounds().height);
                cat.setScale(labyrinthCatIdleScale, labyrinthCatIdleScale);
                cat.setPosition(
                    position.x, position.y + labyrinthIdleVisualYOffset);
                labyrinthIdleOffsetApplied = true;
                catAnimClock.restart();
            }
            return;
        }

        if (cat.getTexture() != movementTexture)
        {
            if (labyrinthIdleOffsetApplied)
            {
                cat.move(0.0f, -labyrinthIdleVisualYOffset);
                labyrinthIdleOffsetApplied = false;
            }
            *currentFrame = 0;
            cat.setTexture(*movementTexture, true);
            cat.setOrigin(0.0f, cat.getLocalBounds().height);
            cat.setScale(movementScale, movementScale);
            cat.setTextureRect(sf::IntRect(
                0, 0, frameWidth, frameHeight));
            catAnimClock.restart();
        }
        if (catAnimClock.getElapsedTime().asSeconds() >= rightFrameDuration)
        {
            *currentFrame = (*currentFrame + 1) % 4;
            cat.setTextureRect(sf::IntRect(
                *currentFrame * frameWidth, 0, frameWidth, frameHeight));
            catAnimClock.restart();
        }

        const float movementSpeed = 260.0f;
        const float horizontalDirection = movingRight ? 1.0f : -1.0f;
        const float verticalDirection = movingDown ? 1.0f : -1.0f;
        if (movingHorizontal)
        {
            const float previousX = cat.getPosition().x;
            const float rightLimit = labyrinthBounds.left + labyrinthBounds.width -
                cat.getGlobalBounds().width;
            cat.setPosition(std::max(labyrinthBounds.left,
                std::min(previousX + horizontalDirection * movementSpeed * deltaTime,
                    rightLimit)), cat.getPosition().y);
            if (labyrinthCollides(labyrinthCollisionBounds(cat)))
            {
                cat.setPosition(previousX, cat.getPosition().y);
            }
        }
        if (movingVertical)
        {
            const float previousY = cat.getPosition().y;
            const float verticalMovementMargin = labyrinthBounds.height * 5.0f /
                static_cast<float>(labyrinthRows);
            const float topLimit = labyrinthBounds.top - verticalMovementMargin +
                cat.getGlobalBounds().height;
            const float bottomLimit = labyrinthBounds.top + labyrinthBounds.height +
                verticalMovementMargin;
            cat.setPosition(cat.getPosition().x, std::max(topLimit,
                std::min(previousY + verticalDirection * movementSpeed * deltaTime,
                    bottomLimit)));
            if (labyrinthCollides(labyrinthCollisionBounds(cat)))
            {
                cat.setPosition(cat.getPosition().x, previousY);
            }
        }
    };
    const float energyDrainInterval = 30.0f;
    float movementEnergyTime = 0.0f;
    sf::Clock foodWarningClock;
    sf::Clock zeroEnergyClock;
    sf::Clock sickDeathClock;
    bool foodWarningStarted = false;
    bool zeroEnergyTimerStarted = false;
    bool sickDeathTimerStarted = false;
    bool sick = false;
    bool gameOver = false;
    bool veterinarianScreen = false;
    bool labyrinthScreen = false;
    int selectedGameOverOption = 0;
    std::string gameOverState = "HAMBRIENTO";
    const float baseCatSpeed = 220.0f;
    const float healthTransitionDuration = 0.8f;
    const float healthTransitionOffset = 180.0f;
    int healthState = 0;
    bool healthTransitioning = false;
    std::vector<std::string> healthHistory{"SANO"};

    auto healthStateName = [](int state)
    {
        return state == 4 ? std::string("ENFERMO") :
            (state == 2 ? std::string("HAMBRIENTO") :
            (state == 3 ? std::string("NECESITA IR AL BAÑO") :
            (state == 0 ? std::string("SANO") : std::string("CANSADO"))));
    };

    auto enterGameOver = [&](const std::string& state)
    {
        if (!gameOver)
        {
            gameOverState = state;
            gato.ReproducirVideo(
                "assets/Images/video Angel.mp4",
                window.getSystemHandle());
            gameOver = true;
            music.stop();
            gameOverMusic.play();
        }
    };

    while (window.isOpen())
    {
        const float deltaTime = frameClock.restart().asSeconds();
        bool energyKeyPressed = false;
        bool gameOverKeyPressed = false;
        bool sleepKeyPressed = false;
        bool eatKeyPressed = false;
        bool bathroomKeyPressed = false;
        bool restartKeyPressed = false;
        bool enterKeyPressed = false;
        bool upKeyPressed = false;
        bool downKeyPressed = false;
        bool veterinarianKeyPressed = false;
        bool veterinarianMousePressed = false;
        bool labyrinthKeyPressed = false;
        bool escapeKeyPressed = false;

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space &&
                !transitioning && !started && !registrationScreen && !gameOver)
            {
                registrationScreen = true;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left && registrationScreen)
            {
                const sf::Vector2f mousePosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                for (int index = 0; index < 3; ++index)
                {
                    if (registrationFields[index].getGlobalBounds().contains(mousePosition))
                    {
                        selectedRegistrationField = index;
                    }
                }
            }

            if (event.type == sf::Event::TextEntered && registrationScreen &&
                event.text.unicode >= 32 && event.text.unicode < 127 &&
                registrationValuesText[selectedRegistrationField].size() < 24)
            {
                registrationValuesText[selectedRegistrationField] +=
                    static_cast<char>(event.text.unicode);
                registrationInstruction.setString(registrationInstructionDefault);
            }

            if (event.type == sf::Event::KeyPressed && registrationScreen &&
                event.key.code == sf::Keyboard::BackSpace &&
                !registrationValuesText[selectedRegistrationField].empty())
            {
                registrationValuesText[selectedRegistrationField].pop_back();
                registrationInstruction.setString(registrationInstructionDefault);
            }

            if (event.type == sf::Event::KeyPressed && registrationScreen &&
                event.key.code == sf::Keyboard::Enter &&
                !registrationValuesText[0].empty() &&
                !registrationValuesText[1].empty() &&
                !registrationValuesText[2].empty())
            {
                if (dueno.RegistrarGato(
                        registrationValuesText[0],
                        registrationValuesText[1],
                        registrationValuesText[2],
                        gato))
                {
                    registrationScreen = false;
                    transitioning = true;
                    transitionClock.restart();
                    registrationInstruction.setString(registrationInstructionDefault);
                }
                else
                {
                    registrationInstruction.setString(
                        "Edad invalida: ingresa un numero entre 1 y 30");
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && gameOver && !veterinarianScreen)
            {
                restartKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && gameOver && !veterinarianScreen)
            {
                enterKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up && gameOver && !veterinarianScreen)
            {
                upKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down && gameOver && !veterinarianScreen)
            {
                downKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1 && started)
            {
                energyKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3 && started)
            {
                gameOverKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E && started)
            {
                sleepKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C && started)
            {
                eatKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M && started)
            {
                bathroomKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V && started)
            {
                veterinarianKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && started)
            {
                labyrinthKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && labyrinthScreen)
            {
                escapeKeyPressed = true;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left && started)
            {
                const sf::Vector2f mousePosition = window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                veterinarianMousePressed = veterinarianOption.getGlobalBounds().contains(mousePosition);
            }
        }

        window.clear(sf::Color::Black);

        if (restartKeyPressed || (enterKeyPressed && selectedGameOverOption == 0))
        {
            gato.Dormir();
            gato.Comer(comida);
            foodWarningStarted = false;
            movementEnergyTime = 0.0f;
            zeroEnergyTimerStarted = false;
            sickDeathTimerStarted = false;
            sick = false;
            gameOver = false;
            veterinarianScreen = false;
            selectedGameOverOption = 0;
            gameOverMusic.stop();
            music.play();
            started = false;
            transitioning = false;
            healthState = 0;
            healthTransitioning = false;
            healthHistory.clear();
            healthHistory.push_back("SANO");
            for (sf::Sprite& energySprite : energySprites)
            {
                energySprite.setTextureRect(
                    sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
            }
            gato.Mover(cat, catIdleTexture, catRightTexture, catLeftTexture,
                catRightFrameWidth, catRightFrameHeight, catLeftFrameWidth,
                catLeftFrameHeight, catScale, catRightScale, catLeftScale,
                catGroundY, catRightGroundY, catLeftGroundY, backgroundBounds,
                catAnimClock, currentRightFrame, currentLeftFrame,
                rightFrameDuration, leftFrameDuration, false, false, 0.0f, 0.0f);
            cat.setPosition(
                backgroundBounds.left + backgroundBounds.width * 0.47f,
                catGroundY);
        }

        if (gameOver && (upKeyPressed || downKeyPressed))
        {
            selectedGameOverOption = selectedGameOverOption == 0 ? 1 : 0;
        }

        if (gameOver && enterKeyPressed && selectedGameOverOption == 1)
        {
            veterinario.RevivirGato(gato, comida, window.getSystemHandle());
            foodWarningStarted = false;
            movementEnergyTime = 0.0f;
            zeroEnergyTimerStarted = false;
            sickDeathTimerStarted = false;
            sick = false;
            gameOver = false;
            veterinarianScreen = false;
            selectedGameOverOption = 0;
            gameOverMusic.stop();
            music.play();
            healthState = 0;
            healthTransitioning = false;
            healthHistory.clear();
            healthHistory.push_back("SANO");
            healthText.setString("SANO");
            healthText.setPosition(
                static_cast<float>(window.getSize().x) / 2.0f,
                static_cast<float>(window.getSize().y) - 24.0f);
            healthText.setOrigin(
                healthText.getLocalBounds().left + healthText.getLocalBounds().width / 2.0f,
                healthText.getLocalBounds().top + healthText.getLocalBounds().height / 2.0f);
            for (sf::Sprite& energySprite : energySprites)
            {
                energySprite.setTextureRect(
                    sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
            }
        }

        if (registrationScreen)
        {
            for (int index = 0; index < 3; ++index)
            {
                registrationValues[index].setString(registrationValuesText[index]);
                registrationFields[index].setOutlineColor(
                    selectedRegistrationField == index ? sf::Color::Yellow : sf::Color::White);
            }
            window.draw(registrationBackground);
            window.draw(registrationTitle);
            for (int index = 0; index < 3; ++index)
            {
                window.draw(registrationLabels[index]);
                window.draw(registrationFields[index]);
                window.draw(registrationValues[index]);
            }
            window.draw(registrationInstruction);
        }
        else if (veterinarianScreen)
        {
            window.clear(sf::Color::Black);
            window.draw(veterinarianText);
        }
        else if (labyrinthScreen)
        {
            if (escapeKeyPressed)
            {
                labyrinthScreen = false;
                cat.setTexture(catIdleTexture, true);
                cat.setOrigin(0.0f, cat.getLocalBounds().height);
                cat.setScale(catScale, catScale);
                cat.setPosition(
                    backgroundBounds.left + backgroundBounds.width * 0.47f,
                    catGroundY);
            }

            const bool labyrinthMovingRight =
                sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            const bool labyrinthMovingLeft =
                sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            const bool labyrinthMovingUp =
                sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            const bool labyrinthMovingDown =
                sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            if (!escapeKeyPressed)
            {
                moveLabyrinthCat(labyrinthMovingRight, labyrinthMovingLeft,
                    labyrinthMovingUp, labyrinthMovingDown, deltaTime);
            }

            window.draw(labyrinthBackground);
            window.draw(cat);
        }
        else if (gameOver)
        {
            restartOption.setOutlineColor(selectedGameOverOption == 0 ? sf::Color::Yellow : sf::Color::White);
            reviveOption.setOutlineColor(selectedGameOverOption == 1 ? sf::Color::Yellow : sf::Color::White);
            gameOverStateText.setString("pasaste de " + gameOverState + " a MUERTO");
            gameOverStateText.setPosition(
                static_cast<float>(window.getSize().x) / 2.0f,
                385.0f);
            gameOverStateText.setOrigin(
                gameOverStateText.getLocalBounds().left + gameOverStateText.getLocalBounds().width / 2.0f,
                gameOverStateText.getLocalBounds().top + gameOverStateText.getLocalBounds().height / 2.0f);
            window.draw(gameOverBackground);
            window.draw(gameOverStateText);
            window.draw(restartOption);
            window.draw(reviveOption);
            window.draw(restartText);
            window.draw(reviveText);
        }
        else if (started)
        {
            const bool movingRight =
                sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            const bool movingLeft =
                sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            const bool moving = movingRight != movingLeft;
            const int energy = gato.ObtenerEnergia();
            const bool nearBed = cama.EstaCerca(cat.getPosition().x);
            const bool nearFood = comida.CercaDeComida(cat.getPosition().x);
            const bool nearBathroom = arenero.EstaCerca(cat.getPosition().x);
            const float doorX = backgroundBounds.left + backgroundBounds.width * 0.5f;
            const bool nearDoor = std::abs(cat.getPosition().x - doorX) <=
                backgroundBounds.width * 0.12f;

            if (labyrinthKeyPressed && energy > 0 && nearDoor)
            {
                labyrinthScreen = true;
                cat.setTexture(catIdleTexture, true);
                cat.setOrigin(0.0f, cat.getLocalBounds().height);
                cat.setScale(labyrinthCatIdleScale, labyrinthCatIdleScale);
                cat.setPosition(
                    labyrinthBackground.getGlobalBounds().left +
                        labyrinthBackground.getGlobalBounds().width / 2.0f -
                        cat.getGlobalBounds().width / 2.0f,
                    labyrinthBackground.getGlobalBounds().top +
                        labyrinthBackground.getGlobalBounds().height * 0.54f +
                        cat.getGlobalBounds().height / 2.0f);
            }

            if (energy == 0 && !zeroEnergyTimerStarted)
            {
                zeroEnergyClock.restart();
                zeroEnergyTimerStarted = true;
            }

            if (energy > 0)
            {
                zeroEnergyTimerStarted = false;
                sick = false;
            }

            const float zeroEnergySeconds = zeroEnergyTimerStarted ?
                zeroEnergyClock.getElapsedTime().asSeconds() : 0.0f;
            if (!sick && zeroEnergySeconds >= 30.0f)
            {
                sick = true;
                sickDeathClock.restart();
                sickDeathTimerStarted = true;
            }

            if (sick && sickDeathTimerStarted &&
                sickDeathClock.getElapsedTime().asSeconds() >= 30.0f)
            {
                enterGameOver("ENFERMO");
            }

            if (sick && (veterinarianKeyPressed || veterinarianMousePressed))
            {
                veterinario.CurarGato(gato, window.getSystemHandle());
                for (sf::Sprite& energySprite : energySprites)
                {
                    energySprite.setTextureRect(
                        sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
                }
                movementEnergyTime = 0.0f;
                zeroEnergyTimerStarted = false;
                sickDeathTimerStarted = false;
                sick = false;
            }

            if (bathroomKeyPressed && nearBathroom)
            {
                gato.IrAlBano(arenero, comida, window.getSystemHandle(),
                    cat.getPosition().x);
            }

            if (eatKeyPressed && gato.Comer(comida, "assets/Images/Comer.mp4",
                window.getSystemHandle(), cat.getPosition().x))
            {
                foodWarningStarted = false;
            }

            const int food = comida.ObtenerCantidad();

            if (food == 0 && !foodWarningStarted)
            {
                foodWarningClock.restart();
                foodWarningStarted = true;
            }

            if (gameOverKeyPressed)
            {
                enterGameOver(food == 0 ? "HAMBRIENTO" :
                    (food == 3 ? "NECESITA IR AL BAÑO" :
                    (energy > 0 ? "SANO" : "CANSADO")));
            }

            if (food == 0)
            {
                const int secondsRemaining = std::max(
                    0, 30 - static_cast<int>(foodWarningClock.getElapsedTime().asSeconds()));
                foodText.setString("ve a comer " + std::to_string(secondsRemaining) + "s");
                foodText.setOrigin(
                    foodText.getLocalBounds().left + foodText.getLocalBounds().width / 2.0f,
                    foodText.getLocalBounds().top);

                if (secondsRemaining == 0)
                {
                    enterGameOver("HAMBRIENTO");
                }
            }

            if (!sick && sleepKeyPressed && gato.Dormir(cama, "assets/Images/Dormir.mp4",
                window.getSystemHandle(), cat.getPosition().x))
            {
                window.setActive(true);
                window.clear(sf::Color::Black);
                window.display();
                movementEnergyTime = 0.0f;
                for (sf::Sprite& energySprite : energySprites)
                {
                    energySprite.setTextureRect(
                        sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
                }
                gato.Mover(cat, catIdleTexture, catRightTexture, catLeftTexture,
                    catRightFrameWidth, catRightFrameHeight, catLeftFrameWidth,
                    catLeftFrameHeight, catScale, catRightScale, catLeftScale,
                    catGroundY, catRightGroundY, catLeftGroundY, backgroundBounds,
                    catAnimClock, currentRightFrame, currentLeftFrame,
                    rightFrameDuration, leftFrameDuration, false, false, 0.0f, 0.0f);
                currentRightFrame = 0;
                currentLeftFrame = 0;
                catAnimClock.restart();
            }

            if (energyKeyPressed && gato.ObtenerEnergia() > 0)
            {
                gato.PerderEnergia();
                energySprites[gato.ObtenerEnergia()].setTextureRect(
                    sf::IntRect(0, 0, energyFrameWidth, energyFrameHeight));
            }

            if (moving)
            {
                movementEnergyTime += deltaTime;
                while (movementEnergyTime >= energyDrainInterval && gato.ObtenerEnergia() > 0)
                {
                    movementEnergyTime -= energyDrainInterval;
                    gato.PerderEnergia();
                    energySprites[gato.ObtenerEnergia()].setTextureRect(
                        sf::IntRect(0, 0, energyFrameWidth, energyFrameHeight));
                }
            }

            const bool isHealthy = gato.ObtenerEnergia() > 0;
            const int currentHealthState = sick ? 4 : (!isHealthy ? 1 :
                (food == 0 ? 2 : (food == 3 ? 3 : 0)));
            const std::string currentState = healthStateName(currentHealthState);
            if (currentHealthState != healthState)
            {
                healthHistory.push_back(currentState);
                healthText.setString(currentState);
                healthTransitionClock.restart();
                healthTransitioning = true;
                healthState = currentHealthState;
            }

            if (healthTransitioning)
            {
                const float progress = std::min(
                    healthTransitionClock.getElapsedTime().asSeconds() /
                        healthTransitionDuration,
                    1.0f);
                const float transitionX = static_cast<float>(window.getSize().x) / 2.0f -
                    healthTransitionOffset * (1.0f - progress);
                healthText.setPosition(transitionX, static_cast<float>(window.getSize().y) - 24.0f);
                healthText.setOrigin(
                    healthText.getLocalBounds().left + healthText.getLocalBounds().width / 2.0f,
                    healthText.getLocalBounds().top + healthText.getLocalBounds().height / 2.0f);

                if (progress >= 1.0f)
                {
                    healthText.setString(currentState);
                    healthText.setPosition(
                        static_cast<float>(window.getSize().x) / 2.0f,
                        static_cast<float>(window.getSize().y) - 24.0f);
                    healthText.setOrigin(
                        healthText.getLocalBounds().left + healthText.getLocalBounds().width / 2.0f,
                        healthText.getLocalBounds().top + healthText.getLocalBounds().height / 2.0f);
                    healthTransitioning = false;
                }
            }

            const float catSpeed = sick ? baseCatSpeed * 0.1f : baseCatSpeed *
                (0.4f + 0.2f * static_cast<float>(gato.ObtenerEnergia()));
            gato.Mover(cat, catIdleTexture, catRightTexture, catLeftTexture,
                catRightFrameWidth, catRightFrameHeight, catLeftFrameWidth,
                catLeftFrameHeight, catScale, catRightScale, catLeftScale,
                catGroundY, catRightGroundY, catLeftGroundY, backgroundBounds,
                catAnimClock, currentRightFrame, currentLeftFrame,
                rightFrameDuration, leftFrameDuration, movingRight, movingLeft,
                deltaTime, catSpeed);

            window.draw(background);
            if (sick)
            {
                window.draw(sadOverlay);
            }
            window.draw(cat);
            for (const sf::Sprite& energySprite : energySprites)
            {
                window.draw(energySprite);
            }
            comida.Dibujar(window);
            if (food == 0)
            {
                window.draw(foodText);
                if (nearFood)
                {
                    comida.DibujarAviso(window);
                }
            }
            else if (nearBathroom)
            {
                arenero.DibujarAviso(window);
            }
            else if (gato.ObtenerEnergia() == 0 && !sick)
            {
                if (nearBed)
                {
                    cama.DibujarAviso(window);
                }
            }
            if (gato.ObtenerEnergia() == 0 && !sick && zeroEnergyTimerStarted)
            {
                const int secondsRemaining = std::max(
                    0, 30 - static_cast<int>(zeroEnergySeconds));
                sickTimerText.setString("enferma en " + std::to_string(secondsRemaining) + "s");
                sickTimerText.setOrigin(
                    sickTimerText.getLocalBounds().left + sickTimerText.getLocalBounds().width / 2.0f,
                    sickTimerText.getLocalBounds().top);
                window.draw(sickTimerText);
            }
            if (sick)
            {
                window.draw(veterinarianOption);
                window.draw(veterinarianOptionText);
            }
            if (energy > 0 && nearDoor)
            {
                window.draw(labyrinthOptionText);
            }

            window.draw(healthText);
            float stateLeft = healthText.getGlobalBounds().left;
            for (std::size_t index = healthHistory.size() - 1; index > 0; --index)
            {
                sf::Text arrow("->", titleFont, 22);
                arrow.setFillColor(sf::Color(220, 220, 220));
                const float arrowWidth = arrow.getLocalBounds().width;
                arrow.setPosition(stateLeft - 16.0f - arrowWidth / 2.0f,
                    static_cast<float>(window.getSize().y) - 24.0f);
                arrow.setOrigin(
                    arrow.getLocalBounds().left + arrow.getLocalBounds().width / 2.0f,
                    arrow.getLocalBounds().top + arrow.getLocalBounds().height / 2.0f);

                sf::Text previousState(healthHistory[index - 1], titleFont, 22);
                previousState.setFillColor(sf::Color(180, 180, 180));
                const float previousWidth = previousState.getLocalBounds().width;
                previousState.setPosition(
                    arrow.getPosition().x - arrowWidth / 2.0f - 12.0f - previousWidth / 2.0f,
                    static_cast<float>(window.getSize().y) - 24.0f);
                previousState.setOrigin(
                    previousState.getLocalBounds().left + previousState.getLocalBounds().width / 2.0f,
                    previousState.getLocalBounds().top + previousState.getLocalBounds().height / 2.0f);

                window.draw(previousState);
                window.draw(arrow);
                stateLeft = previousState.getGlobalBounds().left;
            }
        }
        else if (transitioning)
        {
            const float elapsed = transitionClock.getElapsedTime().asSeconds();
            const float duration = 1.2f;
            const float progress = std::min(elapsed / duration, 1.0f);

            const sf::Uint8 coverAlpha = static_cast<sf::Uint8>(255.0f * (1.0f - progress));
            const sf::Uint8 backgroundAlpha = static_cast<sf::Uint8>(255.0f * progress);

            background.setColor(sf::Color(255, 255, 255, backgroundAlpha));
            cover.setColor(sf::Color(255, 255, 255, coverAlpha));
            cat.setColor(sf::Color(255, 255, 255, backgroundAlpha));

            window.draw(background);
            window.draw(cat);
            window.draw(cover);

            if (progress >= 1.0f)
            {
                started = true;
                transitioning = false;
                background.setColor(sf::Color::White);
                cover.setColor(sf::Color::White);
                cat.setColor(sf::Color::White);
            }
        }
        else
        {
            window.draw(cover);
            window.draw(pressText);
        }

        window.display();
    }

    return 0;
}

int main()
{
    return EjecutarJuego();
}
