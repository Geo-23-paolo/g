#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>

#include "Gato.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gatochi");
    window.setKeyRepeatEnabled(false);
    Gato gato("Gatochi", 1);

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
    fitSpriteToWindow(cover, window);
    fitSpriteToWindow(background, window);

    const sf::Vector2u catSize = catIdleTexture.getSize();
    const sf::Vector2u energySheetSize = energyTexture.getSize();
    const int energyFrameWidth = static_cast<int>(energySheetSize.x / 2);
    const int energyFrameHeight = static_cast<int>(energySheetSize.y);
    const sf::Vector2u foodSheetSize = foodTexture.getSize();
    const int foodFrameWidth = static_cast<int>(foodSheetSize.x / 2);
    const int foodFrameHeight = static_cast<int>(foodSheetSize.y);
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

    sf::Sprite foodSprites[3];
    const float foodScale = 0.10f;
    const float foodMargin = 20.0f;
    for (int index = 0; index < 3; ++index)
    {
        foodSprites[index].setTexture(foodTexture);
        foodSprites[index].setTextureRect(
            sf::IntRect(foodFrameWidth, 0, foodFrameWidth, foodFrameHeight));
        foodSprites[index].setScale(foodScale, foodScale);
        foodSprites[index].setPosition(
            foodMargin + foodFrameWidth * foodScale * static_cast<float>(index),
            foodMargin);
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

    sf::Text sleepText;
    sleepText.setFont(titleFont);
    sleepText.setString("ve a dormir");
    sleepText.setCharacterSize(std::max(26u, static_cast<unsigned int>(window.getSize().y / 18u)));
    sleepText.setFillColor(sf::Color::White);
    sleepText.setStyle(sf::Text::Bold);
    sleepText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        18.0f);
    sleepText.setOrigin(
        sleepText.getLocalBounds().left + sleepText.getLocalBounds().width / 2.0f,
        sleepText.getLocalBounds().top);

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

    sf::Text foodPrompt;
    foodPrompt.setFont(titleFont);
    foodPrompt.setString("Presiona C para comer");
    foodPrompt.setCharacterSize(22);
    foodPrompt.setFillColor(sf::Color::White);
    foodPrompt.setStyle(sf::Text::Bold);
    foodPrompt.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.61f,
        backgroundBounds.top + backgroundBounds.height * 0.56f);
    foodPrompt.setOrigin(
        foodPrompt.getLocalBounds().left + foodPrompt.getLocalBounds().width / 2.0f,
        foodPrompt.getLocalBounds().top);

    sf::Text gameOverText;
    gameOverText.setFont(titleFont);
    gameOverText.setString("game over\n\nPresione espacio para volver al inicio");
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(
        static_cast<float>(window.getSize().x) / 2.0f,
        static_cast<float>(window.getSize().y) / 2.0f);
    gameOverText.setOrigin(
        gameOverText.getLocalBounds().left + gameOverText.getLocalBounds().width / 2.0f,
        gameOverText.getLocalBounds().top + gameOverText.getLocalBounds().height / 2.0f);

    sf::Text sleepPrompt;
    sleepPrompt.setFont(titleFont);
    sleepPrompt.setString("Presiona E para dormir");
    sleepPrompt.setCharacterSize(22);
    sleepPrompt.setFillColor(sf::Color::White);
    sleepPrompt.setStyle(sf::Text::Bold);
    sleepPrompt.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.40f,
        backgroundBounds.top + backgroundBounds.height * 0.56f);
    sleepPrompt.setOrigin(
        sleepPrompt.getLocalBounds().left + sleepPrompt.getLocalBounds().width / 2.0f,
        sleepPrompt.getLocalBounds().top);

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
    const float foodDrainInterval = 30.0f;
    float foodTime = 0.0f;
    int food = 3;
    sf::Clock foodWarningClock;
    bool foodWarningStarted = false;
    bool gameOver = false;
    const float baseCatSpeed = 220.0f;
    const float sleepZoneRight = backgroundBounds.left + backgroundBounds.width * 0.22f;
    const float foodZoneLeft = backgroundBounds.left + backgroundBounds.width * 0.50f;
    const float foodZoneRight = backgroundBounds.left + backgroundBounds.width * 0.75f;
    const float healthTransitionDuration = 0.8f;
    const float healthTransitionOffset = 180.0f;
    int healthState = 0;
    bool healthTransitioning = false;

    auto setCatIdle = [&cat, &catIdleTexture, catScale, backgroundBounds, catGroundY]()
    {
        const sf::Vector2f basePosition = cat.getPosition();
        cat.setTexture(catIdleTexture, true);
        cat.setOrigin(0.0f, cat.getLocalBounds().height);
        cat.setScale(catScale, catScale);

        const float idleRightLimit =
            backgroundBounds.left + backgroundBounds.width - cat.getGlobalBounds().width;
        const float clampedX = std::max(backgroundBounds.left, std::min(basePosition.x, idleRightLimit));
        cat.setPosition(clampedX, catGroundY);
    };

    auto setCatRightFrame = [&cat, &catRightTexture, catRightFrameWidth, catRightFrameHeight, catRightScale, catRightGroundY](int frame)
    {
        const sf::Vector2f basePosition = cat.getPosition();
        cat.setTexture(catRightTexture, true);
        cat.setTextureRect(sf::IntRect(frame * catRightFrameWidth, 0, catRightFrameWidth, catRightFrameHeight));
        cat.setOrigin(0.0f, cat.getLocalBounds().height);

        cat.setScale(catRightScale, catRightScale);
        cat.setPosition(basePosition.x, catRightGroundY);
    };

    auto setCatLeftFrame = [&cat, &catLeftTexture, catLeftFrameWidth, catLeftFrameHeight, catLeftScale, catLeftGroundY](int frame)
    {
        const sf::Vector2f basePosition = cat.getPosition();
        cat.setTexture(catLeftTexture, true);
        cat.setTextureRect(sf::IntRect(frame * catLeftFrameWidth, 0, catLeftFrameWidth, catLeftFrameHeight));
        cat.setOrigin(0.0f, cat.getLocalBounds().height);
        cat.setScale(catLeftScale, catLeftScale);
        cat.setPosition(basePosition.x, catLeftGroundY);
    };

    while (window.isOpen())
    {
        const float deltaTime = frameClock.restart().asSeconds();
        bool energyKeyPressed = false;
        bool foodKeyPressed = false;
        bool gameOverKeyPressed = false;
        bool sleepKeyPressed = false;
        bool eatKeyPressed = false;
        bool restartKeyPressed = false;

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

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && gameOver)
            {
                restartKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1 && started)
            {
                energyKeyPressed = true;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2 && started)
            {
                foodKeyPressed = true;
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
        }

        window.clear(sf::Color::Black);

        if (restartKeyPressed)
        {
            gato.Dormir();
            food = 3;
            foodTime = 0.0f;
            foodWarningStarted = false;
            movementEnergyTime = 0.0f;
            gameOver = false;
            started = false;
            transitioning = false;
            healthState = 0;
            healthTransitioning = false;
            for (sf::Sprite& energySprite : energySprites)
            {
                energySprite.setTextureRect(
                    sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
            }
            for (sf::Sprite& foodSprite : foodSprites)
            {
                foodSprite.setTextureRect(
                    sf::IntRect(foodFrameWidth, 0, foodFrameWidth, foodFrameHeight));
            }
            setCatIdle();
            cat.setPosition(
                backgroundBounds.left + backgroundBounds.width * 0.47f,
                catGroundY);
        }

        if (gameOver)
        {
            window.draw(gameOverText);
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
            const bool nearBed = cat.getPosition().x <= sleepZoneRight;
            const bool canSleep = energy == 0 && nearBed;
            const bool nearFood = cat.getPosition().x >= foodZoneLeft &&
                cat.getPosition().x <= foodZoneRight;
            const bool canEat = food == 0 && nearFood;

            if (foodKeyPressed && food > 0)
            {
                --food;
                foodSprites[food].setTextureRect(
                    sf::IntRect(0, 0, foodFrameWidth, foodFrameHeight));
            }

            foodTime += deltaTime;
            while (foodTime >= foodDrainInterval && food > 0)
            {
                foodTime -= foodDrainInterval;
                --food;
                foodSprites[food].setTextureRect(
                    sf::IntRect(0, 0, foodFrameWidth, foodFrameHeight));
            }

            if (food == 0 && !foodWarningStarted)
            {
                foodWarningClock.restart();
                foodWarningStarted = true;
            }

            if (gameOverKeyPressed)
            {
                gameOver = true;
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
                    gameOver = true;
                }
            }

            if (eatKeyPressed && canEat)
            {
                gato.Comer("assets/Images/Comer.mp4", window.getSystemHandle());
                food = 3;
                foodTime = 0.0f;
                foodWarningStarted = false;
                for (sf::Sprite& foodSprite : foodSprites)
                {
                    foodSprite.setTextureRect(
                        sf::IntRect(foodFrameWidth, 0, foodFrameWidth, foodFrameHeight));
                }
            }

            if (sleepKeyPressed && canSleep)
            {
                gato.Dormir("assets/Images/Dormir.mp4", window.getSystemHandle());
                window.setActive(true);
                window.clear(sf::Color::Black);
                window.display();
                movementEnergyTime = 0.0f;
                for (sf::Sprite& energySprite : energySprites)
                {
                    energySprite.setTextureRect(
                        sf::IntRect(energyFrameWidth, 0, energyFrameWidth, energyFrameHeight));
                }
                setCatIdle();
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
            const int currentHealthState = food == 0 ? 2 : (isHealthy ? 0 : 1);
            if (currentHealthState != healthState)
            {
                const std::string previousState = healthState == 0 ? "SANO" :
                    (healthState == 1 ? "CANSADO" : "HAMBRIENTO");
                const std::string currentState = currentHealthState == 2 ? "HAMBRIENTO" :
                    (currentHealthState == 0 ? "SANO" : "CANSADO");
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
                        (currentHealthState == 0 ? "SANO" : "CANSADO"));
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

            if (movingRight && !movingLeft)
            {
                if (cat.getTexture() != &catRightTexture)
                {
                    currentRightFrame = 0;
                    setCatRightFrame(currentRightFrame);
                    catAnimClock.restart();
                }

                if (catAnimClock.getElapsedTime().asSeconds() >= rightFrameDuration)
                {
                    currentRightFrame = (currentRightFrame + 1) % catRightFrameCount;
                    setCatRightFrame(currentRightFrame);
                    catAnimClock.restart();
                }

                const float rightLimit =
                    backgroundBounds.left + backgroundBounds.width - cat.getGlobalBounds().width;
                const float newX = std::min(cat.getPosition().x + catSpeed * deltaTime, rightLimit);
                cat.setPosition(newX, catRightGroundY);
            }
            else if (movingLeft && !movingRight)
            {
                if (cat.getTexture() != &catLeftTexture)
                {
                    currentLeftFrame = 0;
                    setCatLeftFrame(currentLeftFrame);
                    catAnimClock.restart();
                }

                if (catAnimClock.getElapsedTime().asSeconds() >= leftFrameDuration)
                {
                    currentLeftFrame = (currentLeftFrame + 1) % catLeftFrameCount;
                    setCatLeftFrame(currentLeftFrame);
                    catAnimClock.restart();
                }

                const float leftLimit = backgroundBounds.left;
                const float newX = std::max(cat.getPosition().x - catSpeed * deltaTime, leftLimit);
                cat.setPosition(newX, catLeftGroundY);
            }
            else if (cat.getTexture() != &catIdleTexture)
            {
                setCatIdle();
                currentRightFrame = 0;
                currentLeftFrame = 0;
                catAnimClock.restart();
            }

            window.draw(background);
            window.draw(cat);
            for (const sf::Sprite& energySprite : energySprites)
            {
                window.draw(energySprite);
            }
            for (const sf::Sprite& foodSprite : foodSprites)
            {
                window.draw(foodSprite);
            }
            if (food == 0)
            {
                window.draw(foodText);
                if (nearFood)
                {
                    window.draw(foodPrompt);
                }
            }
            else if (gato.ObtenerEnergia() == 0)
            {
                window.draw(sleepText);
                if (nearBed)
                {
                    window.draw(sleepPrompt);
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
