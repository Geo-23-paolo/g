#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "Arenero.hpp"
#include "Cama.hpp"
#include "Comida.hpp"
#include "Gato.hpp"
#include "Veterinario.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gatochi");
    window.setKeyRepeatEnabled(false);
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
    fitSpriteToWindow(cover, window);
    fitSpriteToWindow(background, window);
    fitSpriteToWindow(gameOverBackground, window);
    fitSpriteToWindow(registrationBackground, window);

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
    registrationInstruction.setFont(titleFont);
    registrationInstruction.setString("Completa los datos y presiona Enter");
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
    const float rightFrameDuration = 0.10f;
    const float leftFrameDuration = 0.10f;
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
            }

            if (event.type == sf::Event::KeyPressed && registrationScreen &&
                event.key.code == sf::Keyboard::BackSpace &&
                !registrationValuesText[selectedRegistrationField].empty())
            {
                registrationValuesText[selectedRegistrationField].pop_back();
            }

            if (event.type == sf::Event::KeyPressed && registrationScreen &&
                event.key.code == sf::Keyboard::Enter &&
                !registrationValuesText[0].empty() &&
                !registrationValuesText[1].empty() &&
                !registrationValuesText[2].empty())
            {
                registrationScreen = false;
                transitioning = true;
                transitionClock.restart();
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
                arenero.Usar(comida, gato, window.getSystemHandle());
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
