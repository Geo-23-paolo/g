#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>

#include "Arenero.hpp"
#include "Cama.hpp"
#include "Comida.hpp"
#include "Gato.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gatochi");
    window.setKeyRepeatEnabled(false);
    Gato gato("Gatochi", 1);
    Comida comida("comida", 3.0f);
    Arenero arenero(true, "derecha");
    Cama cama;

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
    fitSpriteToWindow(cover, window);
    fitSpriteToWindow(background, window);
    fitSpriteToWindow(gameOverBackground, window);

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
    gato.ConfigurarAvisos(titleFont, backgroundBounds);

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
    int currentRightFrame = 0;
    int currentLeftFrame = 0;
    const float rightFrameDuration = 0.10f;
    const float leftFrameDuration = 0.10f;
    const float energyDrainInterval = 30.0f;
    float movementEnergyTime = 0.0f;
    sf::Clock foodWarningClock;
    bool foodWarningStarted = false;
    bool gameOver = false;
    bool veterinarianScreen = false;
    int selectedGameOverOption = 0;
    std::string gameOverState = "HAMBRIENTO";
    const float baseCatSpeed = 220.0f;
    const float healthTransitionDuration = 0.8f;
    const float healthTransitionOffset = 180.0f;
    int healthState = 0;
    bool healthTransitioning = false;

    auto enterGameOver = [&](const std::string& state)
    {
        if (!gameOver)
        {
            gameOverState = state;
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

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space &&
                !transitioning && !started && !gameOver)
            {
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
        }

        window.clear(sf::Color::Black);

        if (restartKeyPressed || (enterKeyPressed && selectedGameOverOption == 0))
        {
            gato.Dormir();
            gato.Comer(comida);
            foodWarningStarted = false;
            movementEnergyTime = 0.0f;
            gameOver = false;
            veterinarianScreen = false;
            selectedGameOverOption = 0;
            gameOverMusic.stop();
            music.play();
            started = false;
            transitioning = false;
            healthState = 0;
            healthTransitioning = false;
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
            gameOver = false;
            veterinarianScreen = true;
        }

        if (veterinarianScreen)
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
            const bool nearFood = gato.CercaDeComida(cat.getPosition().x);
            const bool nearBathroom = arenero.EstaCerca(cat.getPosition().x);

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

            if (sleepKeyPressed && gato.Dormir(cama, "assets/Images/Dormir.mp4",
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
            const int currentHealthState = food == 0 ? 2 :
                (food == 3 ? 3 : (isHealthy ? 0 : 1));
            if (currentHealthState != healthState)
            {
                const std::string previousState = healthState == 0 ? "SANO" :
                    (healthState == 1 ? "CANSADO" :
                    (healthState == 2 ? "HAMBRIENTO" : "NECESITA IR AL BAÑO"));
                const std::string currentState = currentHealthState == 2 ? "HAMBRIENTO" :
                    (currentHealthState == 3 ? "NECESITA IR AL BAÑO" :
                    (currentHealthState == 0 ? "SANO" : "CANSADO"));
                healthText.setString(previousState + " -> " + currentState);
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
                    healthText.setString(currentHealthState == 2 ? "HAMBRIENTO" :
                        (currentHealthState == 3 ? "NECESITA IR AL BAÑO" :
                        (currentHealthState == 0 ? "SANO" : "CANSADO")));
                    healthText.setPosition(
                        static_cast<float>(window.getSize().x) / 2.0f,
                        static_cast<float>(window.getSize().y) - 24.0f);
                    healthText.setOrigin(
                        healthText.getLocalBounds().left + healthText.getLocalBounds().width / 2.0f,
                        healthText.getLocalBounds().top + healthText.getLocalBounds().height / 2.0f);
                    healthTransitioning = false;
                }
            }

            const float catSpeed = baseCatSpeed *
                (0.4f + 0.2f * static_cast<float>(gato.ObtenerEnergia()));
            gato.Mover(cat, catIdleTexture, catRightTexture, catLeftTexture,
                catRightFrameWidth, catRightFrameHeight, catLeftFrameWidth,
                catLeftFrameHeight, catScale, catRightScale, catLeftScale,
                catGroundY, catRightGroundY, catLeftGroundY, backgroundBounds,
                catAnimClock, currentRightFrame, currentLeftFrame,
                rightFrameDuration, leftFrameDuration, movingRight, movingLeft,
                deltaTime, catSpeed);

            window.draw(background);
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
                    gato.DibujarAvisoComer(window);
                }
            }
            else if (nearBathroom)
            {
                arenero.DibujarAviso(window);
            }
            else if (gato.ObtenerEnergia() == 0)
            {
                if (nearBed)
                {
                    gato.DibujarAvisoDormir(window);
                }
            }
            window.draw(healthText);
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
