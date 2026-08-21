#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Gatochi");
    window.setKeyRepeatEnabled(false);

    sf::Texture coverTexture;
    if (!coverTexture.loadFromFile("assets/Images/gatochi.jpg"))
    {
        return 1;
    }

    sf::Texture mainTexture;
    if (!mainTexture.loadFromFile("assets/Images/FondoPrincipal.jpg"))
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
    bool transitioning = false;
    bool started = false;
    int currentRightFrame = 0;
    int currentLeftFrame = 0;
    const float catSpeed = 220.0f;
    const float rightFrameDuration = 0.10f;
    const float leftFrameDuration = 0.10f;

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

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !transitioning && !started)
            {
                transitioning = true;
                transitionClock.restart();
            }
        }

        window.clear(sf::Color::Black);

        if (started)
        {
            const bool movingRight =
                sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            const bool movingLeft =
                sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

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
