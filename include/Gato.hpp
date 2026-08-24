#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Personaje.hpp"

class Cama;
class Comida;
class Arenero;

class Gato : public Personaje
{
public:
    Gato(const std::string& nombre, int edad);

    void Comer(Comida& comida);
    bool Comer(Comida& comida, const std::string& videoPath,
        void* windowHandle, float catX);
    bool IrAlBano(Arenero& arenero, Comida& comida,
        void* windowHandle, float catX);
    void Dormir();
    bool Dormir(const Cama& cama, const std::string& videoPath,
        void* windowHandle, float catX);
    void PerderEnergia();
    int ObtenerEnergia() const;
    void ReproducirVideo(const std::string& videoPath, void* windowHandle);
    void Mover(sf::Sprite& cat, const sf::Texture& idleTexture,
        const sf::Texture& rightTexture, const sf::Texture& leftTexture,
        int rightFrameWidth, int rightFrameHeight, int leftFrameWidth,
        int leftFrameHeight, float idleScale, float rightScale, float leftScale,
        float groundY, float rightGroundY, float leftGroundY,
        const sf::FloatRect& backgroundBounds, sf::Clock& animationClock,
        int& rightFrame, int& leftFrame, float rightFrameDuration,
        float leftFrameDuration, bool movingRight, bool movingLeft,
        float deltaTime, float speed);

private:
    int edad;
    int energia;
};